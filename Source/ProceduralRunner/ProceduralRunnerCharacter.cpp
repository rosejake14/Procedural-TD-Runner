// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProceduralRunnerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "ProceduralRunner/Public/TileSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SNegativeActionButton.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProceduralRunnerCharacter

AProceduralRunnerCharacter::AProceduralRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	spawnyes = true;
	moveSpeed = 8;
	Score = 0;
	specialActive = false;
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 850.f;
	GetCharacterMovement()->AirControl = 1.75f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 10000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 3000.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AProceduralRunnerCharacter::AddScore(int score)
{
	UE_LOG(LogTemp, Warning, TEXT("Score +1"));
	Score += score;
	
}

int AProceduralRunnerCharacter::GetScore()
{
	return Score;
}

void AProceduralRunnerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	if(BGMusic)
	{
		UGameplayStatics::PlaySound2D(this, BGMusic);
	}
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProceduralRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProceduralRunnerCharacter::Move);

		//Activating special abilities
		EnhancedInputComponent->BindAction(Special1, ETriggerEvent::Triggered,this,&AProceduralRunnerCharacter::Special1Activate);

		EnhancedInputComponent->BindAction(Special2, ETriggerEvent::Triggered,this,&AProceduralRunnerCharacter::Special2Activate);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProceduralRunnerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardForce = FVector(50,0,0);
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardForce, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		
	}
}

void AProceduralRunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(FVector(this->GetActorLocation().X , this->GetActorLocation().Y + moveSpeed, this->GetActorLocation().Z), false, 0, ETeleportType::None);
	
}

void AProceduralRunnerCharacter::Special1Activate(const FInputActionValue& Value) //Special: Move slower for 2 seconds.
{
	if(specialActive == false)
	{
		specialActive = true;
		moveSpeed -= 5;
		GetWorldTimerManager().SetTimer(Handle, this, &AProceduralRunnerCharacter::slowTime, 1.0f, true, 2.0f);
	}
}

void AProceduralRunnerCharacter::Special2Activate(const FInputActionValue& Value)  //Special: Single fast, high jump.
{ 
	if(GetCharacterMovement()->IsMovingOnGround()){ //if grounded.
		if(specialActive == false) //Only allow 1 special to be activated at a time.
		{
			specialActive = true;
			tempJump = GetCharacterMovement()->JumpZVelocity;
			GetCharacterMovement()->JumpZVelocity += 1000;
			moveSpeed += 15; 
			GetWorldTimerManager().SetTimer(Handle, this, &AProceduralRunnerCharacter::superJump, 1.0f, true, 3.5f); 
		}
	}
}

void AProceduralRunnerCharacter::slowTime() //End the special ability for slow speed
{
	UE_LOG(LogTemp, Warning, TEXT("Time Slowed"));
	moveSpeed += 5;
	GetWorldTimerManager().ClearTimer(Handle);
	specialActive = false;
}

void AProceduralRunnerCharacter::superJump()
{
	UE_LOG(LogTemp, Warning, TEXT("Super Jump!"));
	GetCharacterMovement()->JumpZVelocity = tempJump; //i dont need a temp here but we move
	moveSpeed -= 15;
	GetWorldTimerManager().ClearTimer(Handle);
	specialActive = false;
}

