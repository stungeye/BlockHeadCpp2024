// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSpawner.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AGridSpawner::AGridSpawner()
{
 	// This actor does not tick:
	PrimaryActorTick.bCanEverTick = false;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grid Instanced Mesh"));
	InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AGridSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGridSpawner::SpawnActor, DelayBetweenSpawns, true);
}

void AGridSpawner::OnConstruction(const FTransform& Transform)
{
	if (InstancedStaticMeshComponent->GetStaticMesh()) {
		InstancedStaticMeshComponent->ClearInstances();

		for (int32 X = 0; X < GridSize; X++) {
			for (int32 Y = 0; Y < GridSize; Y++) {

				FVector Location{ X * GridSpacing, Y * GridSpacing, 0 };
				InstancedStaticMeshComponent->AddInstance(FTransform{ Location });
				UE_LOG(LogTemp, Warning, TEXT("Instance count: %d"), InstancedStaticMeshComponent->GetInstanceCount());
				UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Location.ToString());
			}
		}
	}
}

void AGridSpawner::SpawnActor()
{
	if (ActorToSpawn && SpawnActorCount < MaxSpawnedActorCount ) {
		UE_LOG(LogTemp, Warning, TEXT("SpawnActor"));
		SpawnActorCount++;
		FBoxSphereBounds Bounds = InstancedStaticMeshComponent->CalcBounds(InstancedStaticMeshComponent->GetComponentTransform());
		FVector Location = FMath::RandPointInBox(Bounds.GetBox());
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, FRotator::ZeroRotator);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Timer Cleared"));
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
	
}


