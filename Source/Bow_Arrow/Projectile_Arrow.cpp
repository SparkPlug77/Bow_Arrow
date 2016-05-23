// Fill out your copyright notice in the Description page of Project Settings.

#include "Bow_Arrow.h"
#include "Projectile_Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AProjectile_Arrow::AProjectile_Arrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision_comp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Arrow");
	//CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile_Arrow::OnHit);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	//ProjectileMovement->InitialSpeed = 2000.0f;
	//ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.5f;
	ProjectileMovement->bShouldBounce = false;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	ArrowMesh->AttachTo(RootComponent);

	InitialLifeSpan = 3.0f;

}

void AProjectile_Arrow::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

UProjectileMovementComponent * AProjectile_Arrow::GetProjectileMovementComponent(float)
{
	return ProjectileMovement;
}

UStaticMeshComponent * AProjectile_Arrow::GetStaticMeshComponent()
{
	return ArrowMesh;
}

void AProjectile_Arrow::OnHit(AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity*100.0f, Hit.ImpactPoint);	
	}
}

// Called when the game starts or when spawned
void AProjectile_Arrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile_Arrow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

