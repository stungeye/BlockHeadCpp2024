// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"
#include "Components/BoxComponent.h"

// Sets default values
AEndPoint::AEndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	// Set the plane collisino profile to no collision
	Plane->SetCollisionProfileName(TEXT("NoCollision"));

	Plane->SetupAttachment(Box);
	RootComponent = Box;
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

