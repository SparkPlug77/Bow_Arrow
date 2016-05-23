// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile_Arrow.generated.h"

UCLASS()
class BOW_ARROW_API AProjectile_Arrow : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* ArrowMesh;

protected:

	void DisableAndDestroy();
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Arrow();

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UProjectileMovementComponent* ProjectileMovement;

	void InitVelocity(const FVector & ShootDirection);

	class UProjectileMovementComponent* GetProjectileMovementComponent(float);
	class UStaticMeshComponent* GetStaticMeshComponent();

	UFUNCTION()
	void OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
