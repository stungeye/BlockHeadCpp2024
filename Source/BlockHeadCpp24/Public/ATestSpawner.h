// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATestSpawner.generated.h"

// Forward Declarations:
class UInstancedStaticMeshComponent;

UCLASS()
class BLOCKHEADCPP24_API AATestSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATestSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Construction script:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Uproperty and instanced static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;

	// Make the GridSize instance editable:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 GridSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	double GridSpacing = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	int32 NumSpawnActors = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	double DelayBetweenSpawns = 3.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Protected Variables")
	double ProtectedEditAnywhere = 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Protected Variables")
	double ProtectedVisibleAnywhere = 0.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Protected Variables")
	double ProtectedEditDefaultsOnly = 0.0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Protected Variables")
	double ProtectedVisibleDefaultsOnly = 0.0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Protected Variables")
	double ProtectedEditInstanceOnly = 0.0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Protected Variables")
	double ProtectedVisibleInstanceOnly = 0.0;

private:
	int32 SpawnCount = 0;
	void SpawnInstance();

	FTimerHandle SpawnTimerHandle;

	// UPROP with meta allow private access:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorToSpawn;
};
