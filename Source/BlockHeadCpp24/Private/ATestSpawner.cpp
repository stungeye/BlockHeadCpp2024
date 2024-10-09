// Fill out your copyright notice in the Description page of Project Settings.


#include "ATestSpawner.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AATestSpawner::AATestSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Instanced static mesh component
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	// Turn off collisions for the instanced static mesh component:
	InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AATestSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AATestSpawner::SpawnInstance, DelayBetweenSpawns, true);
}

void AATestSpawner::OnConstruction(const FTransform& Transform)
{
	// If the instanced static mesh component has been assigned a static mesh, add instance:
	if (InstancedStaticMeshComponent->GetStaticMesh()) {
		// Remove all existing instances
		InstancedStaticMeshComponent->ClearInstances();

		for (int32 X = 0; X < GridSize; X++) {
			for (int32 Y = 0; Y < GridSize; Y++) {
				// Calculate the location of the instance:
				FVector Location = FVector{ X * GridSpacing, Y * GridSpacing, 0.0f };
				// Add the instance:
				InstancedStaticMeshComponent->AddInstance(FTransform{ Location });

				UE_LOG(LogTemp, Warning, TEXT("Instance count: %d"), InstancedStaticMeshComponent->GetInstanceCount());
			}
		}
	}
}

void AATestSpawner::SpawnInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Count: %d"), SpawnCount);
	UE_LOG(LogTemp, Warning, TEXT("Num Spawn Actors: %d"), NumSpawnActors);

	// If the actor to spawn has been assigned, spawn it:
	if (ActorToSpawn && (SpawnCount < NumSpawnActors)) {
		SpawnCount++;
		// Get a random location within the bounds of the instanced static mesh component:
		FBoxSphereBounds Bounds = InstancedStaticMeshComponent->CalcBounds(InstancedStaticMeshComponent->GetComponentTransform());
		FVector Origin = Bounds.Origin;
		FVector Extent = Bounds.BoxExtent;
		FVector Location = FMath::RandPointInBox(FBox{ Origin - Extent, Origin + Extent });
		// Spawn the actor:
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, Location, FRotator::ZeroRotator);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Spawning complete"));
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
}


