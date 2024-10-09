// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSpawner.generated.h"

// Forward Declaration. We will include the header in the cpp file.
class UInstancedStaticMeshComponent;

UCLASS()
class BLOCKHEADCPP24_API AGridSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 GridSize{ 10 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	double GridSpacing{ 100.0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	double DelayBetweenSpawns{ 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	int32 MaxSpawnedActorCount{ 100 };
	
private:
	void SpawnActor();

	int32 SpawnActorCount{ 0 };
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorToSpawn;
};
