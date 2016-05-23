// Fill out your copyright notice in the Description page of Project Settings.

#include "Bow_Arrow.h"
#include "MyBow.h"
#include "Engine.h"

// Sets default values
AMyBow::AMyBow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision_Comp"));
	RootComponent = CollisionComp;

	AEBow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow_Weap"));
	AEBow->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AMyBow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

