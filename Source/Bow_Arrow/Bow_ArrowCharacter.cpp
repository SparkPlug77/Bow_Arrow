// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Bow_Arrow.h"
#include "Bow_ArrowCharacter.h"
#include "Engine.h"
#include "Engine/Blueprint.h"
#include "Projectile_Arrow.h"
//////////////////////////////////////////////////////////////////////////
// ABow_ArrowCharacter

ABow_ArrowCharacter::ABow_ArrowCharacter()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBlueprint(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/MyBow.MyBow'"));
	
	WeaponSpawn = NULL;

	if (WeaponBlueprint.Succeeded())
	{
		WeaponSpawn = (UClass*)WeaponBlueprint.Object->GeneratedClass;
	}
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	MaxChargeTime = 3000.0f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->RelativeLocation = FVector(0.0f, 100.0f, 30.0f);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//DrawArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	//static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> ArrowComponent(TEXT("StaticMesh'/Game/MyAssets/Mesh/Weapons/Arrow/Arrow.Arrow'"));
	//DrawArrow->AttachTo(GetMesh(), "ArrowSocket");

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


}

//Void Tick

void ABow_ArrowCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		FollowCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		CameraBoom->TargetArmLength = FMath::Lerp<float>(300.0f, 250.0f, ZoomFactor);

		//if (!bHold && ChargeTime != 0)
		//{
		//	float ChargePercent = FMath::Clamp(ChargeTime, 0.0f, MaxChargeTime)/MaxChargeTime;
		//	float Speed = ChargePercent*MaxArrowSpeed;
		//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Draw"));
		//	ChargeTime = 0;
		//	bHold = false;
		//}
		//else
		//{
		//	ChargeTime += DeltaTime;
		//	/*if (ChargeTime < 3000)
		//	{
		//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Draw"));
		//		ChargeTime += 100;
		//	}*/
		//}

	}
}
void ABow_ArrowCharacter::BeginPlay()
{
	Super::BeginPlay();
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AMyBow* Spawner = GetWorld()->SpawnActor<AMyBow>(WeaponSpawn, SpawnParams);
		if (Spawner)
		{
			Spawner->AttachRootComponentTo(GetMesh(), "GripPoint", EAttachLocation::SnapToTarget);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// Input

void ABow_ArrowCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ABow_ArrowCharacter::Draw);
	InputComponent->BindAction("Fire", IE_Released, this, &ABow_ArrowCharacter::Release);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ABow_ArrowCharacter::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &ABow_ArrowCharacter::ZoomOut);

	InputComponent->BindAxis("MoveForward", this, &ABow_ArrowCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABow_ArrowCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ABow_ArrowCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ABow_ArrowCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ABow_ArrowCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ABow_ArrowCharacter::TouchStopped);
}


void ABow_ArrowCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ABow_ArrowCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ABow_ArrowCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABow_ArrowCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void ABow_ArrowCharacter::Draw()
{
	bHold = true;
	//ArrowSpeed += 100.0f;
	//if (bHold)
	//{
	//	//MaxSpeed += DeltaTime;
	//	if (MaxSpeed <= 3000.0f)
	//	{
	//		MaxSpeed += 100.0f; 
	//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Draw"));
	//	}
	//}
}

void ABow_ArrowCharacter::Release()
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	float DeltaTime = 100;
	if (ChargeTime != 0)
	{
		float ChargePercent = FMath::Clamp(ChargeTime, 0.0f, MaxChargeTime) / MaxChargeTime;
		float Speed = ChargePercent*MaxArrowSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Draw"));
		if (ProjectileClass != NULL)
		{
			FVector CameraLoc;
			FRotator CameraRot;
			GetActorEyesViewPoint(CameraLoc, CameraRot);
			/*FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(GunOffset);*/
			const FVector SocketLocation = UseMesh->GetSocketLocation("ArrowSocket");
			//const FRotator SocketRotation = UseMesh->GetSocketRotation("ArrowSocket");
			FRotator MuzzleRotation = CameraRot;
			MuzzleRotation.Pitch += 10.0f;
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				AProjectile_Arrow* const Projectile = World->SpawnActor<AProjectile_Arrow>(ProjectileClass, SocketLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					FVector const LaunchDir = MuzzleRotation.Vector();
					Projectile->InitVelocity(LaunchDir);
				}
			}
		}
	}
	ChargeTime = 0;
	bHold = false;
	if (bHold)
	{
		ChargeTime += DeltaTime;
	}
}

void ABow_ArrowCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABow_ArrowCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABow_ArrowCharacter::ZoomIn()
{
	bZoomingIn = true;
}

void ABow_ArrowCharacter::ZoomOut()
{
	bZoomingIn = false;
}