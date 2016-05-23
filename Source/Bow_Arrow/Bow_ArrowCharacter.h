// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MyBow.h"
#include "Bow_ArrowCharacter.generated.h"

UCLASS(config=Game)
class ABow_ArrowCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere,  Category = Spawn)
	TSubclassOf<class AMyBow> WeaponSpawn;


public:
	ABow_ArrowCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	AMyBow* CurrentWeapon;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//FName GetAttachPoint() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	FVector GunOffset;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Arrow> ProjectileClass;

	UPROPERTY(BlueprintReadWrite)
	class UStaticMeshComponent* DrawArrow;

protected:

	/** Draw an Arrow. */
	void Draw();

	/**Releases the Arrow. */
	void Release();

	bool bHold;
	float ChargeTime;
	float MaxArrowSpeed;
	float MaxChargeTime;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/**Input functions for Zoom In and Out*/
	float ZoomFactor;
	bool bZoomingIn;

	/**Zoom In and Out to aim*/
	void ZoomIn();
	void ZoomOut();

protected:
	// Called Every Frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

