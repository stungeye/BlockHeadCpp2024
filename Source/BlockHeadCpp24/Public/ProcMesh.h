// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMesh.generated.h"

class UProceduralMeshComponent;

// Enum to select the type of noise to use, 2D vs 3D:
UENUM(BlueprintType)
enum class ENoiseType : uint8
{
	Perlin2D,
	Perlin3D
};


UCLASS()
class BLOCKHEADCPP24_API AProcMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Material selectable from BP:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> ProcMaterial;

	UPROPERTY(EditAnywhere)
	float NoiseScale{ 0.001f };   // This will make the large world space positions small enough for Perlin noise

	UPROPERTY(EditAnywhere)
	float NoiseHeight{ 500.0f };   // Amplitude of the noise

	UPROPERTY(EditAnywhere)
	float NoiseSpeed{ 0.75f };     // Speed of movement through the noise field

	UPROPERTY(EditAnywhere)
	int32 GridSize = 150;			// Number of vertices for the grid length and width	

	UPROPERTY(EditAnywhere)
	float GridSpacing = 100.0f; 	// Spacing between grid vertices

	UPROPERTY(EditAnywhere)
	ENoiseType NoiseType{ ENoiseType::Perlin2D }; // Select 2D or 3D noise

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void GenerateMesh();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> ProceduralMesh;
};
