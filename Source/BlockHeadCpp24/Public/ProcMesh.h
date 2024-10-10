// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcMesh.generated.h"

class UProceduralMeshComponent;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void GenerateMesh();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProceduralMeshComponent> ProceduralMesh;
};
