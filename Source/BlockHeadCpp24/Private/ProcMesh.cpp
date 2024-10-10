// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcMesh.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AProcMesh::AProcMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ProceduralMesh;

}

// Called when the game starts or when spawned
void AProcMesh::BeginPlay()
{
	Super::BeginPlay();
    if (ProcMaterial) {
        ProceduralMesh->SetMaterial(0, ProcMaterial);
    }

	GenerateMesh(); // Call the mesh generation function
}

// Called every frame
void AProcMesh::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Get the current time for animated noise
    float Time = GetWorld()->TimeSeconds;

    FProcMeshSection* MeshSection = ProceduralMesh->GetProcMeshSection(0);
    if (MeshSection)
    {
        TArray<FVector> UpdatedPositions;

        for (int32 i = 0; i < MeshSection->ProcVertexBuffer.Num(); i++)
        {
            FVector VertexPosition = MeshSection->ProcVertexBuffer[i].Position;
            float NoiseValue{ 0.0f };

			if (NoiseType == ENoiseType::Perlin2D)
			{
				FVector2D NoisePosition{
					VertexPosition.X * NoiseScale,  // Scale down X
					(VertexPosition.Y * NoiseScale) + (Time * NoiseSpeed)  // Scale down Y and move over time
				};

				// Use 2D Perlin noise with the scaled-down inputs
				NoiseValue = FMath::PerlinNoise2D(NoisePosition);
			}
			else if (NoiseType == ENoiseType::Perlin3D)
			{

				FVector NoisePosition{
					VertexPosition.X * NoiseScale,  // Scale down X
					(VertexPosition.Y * NoiseScale),
					(Time * NoiseSpeed)  // Move through Z over time
				};
				// Use 3D Perlin noise with the scaled-down inputs
				NoiseValue = FMath::PerlinNoise3D(NoisePosition);
			}

            // Adjust Z value with noise
            VertexPosition.Z = NoiseValue * NoiseHeight;

            UpdatedPositions.Add(VertexPosition);
            /*
            if (i % 100 == 0)  // Log every 100th vertex for clarity
            {
                UE_LOG(LogTemp, Warning, TEXT("Vertex Position: %s"), *VertexPosition.ToString());
                UE_LOG(LogTemp, Warning, TEXT("Noise Position: %s"), *NoisePosition.ToString());
                UE_LOG(LogTemp, Warning, TEXT("NoiseValue: %f"), NoiseValue);
            }
            */
        }
        
        // Update the mesh section with new vertex positions
        ProceduralMesh->UpdateMeshSection(0, UpdatedPositions, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());
    }
}

void AProcMesh::GenerateMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    // Create vertices for a grid
    for (int32 y = 0; y < GridSize; y++)
    {
        for (int32 x = 0; x < GridSize; x++)
        {
            Vertices.Add(FVector(x * GridSpacing, y * GridSpacing, 0.0f)); // Initialize Z to 0
            UV0.Add(FVector2D(x / (float)GridSize, y / (float)GridSize)); // Simple UV mapping
            VertexColors.Add(FLinearColor::White); // Simple white vertex color
            Normals.Add(FVector(0.0f, 0.0f, 1.0f)); // Normals facing up
        }
    }

    // Create triangles (two per quad)
    for (int32 y = 0; y < GridSize - 1; y++)
    {
        for (int32 x = 0; x < GridSize - 1; x++)
        {
            int32 TopLeft = (y * GridSize) + x;
            int32 TopRight = TopLeft + 1;
            int32 BottomLeft = TopLeft + GridSize;
            int32 BottomRight = BottomLeft + 1;

            Triangles.Add(TopLeft);
            Triangles.Add(BottomLeft);
            Triangles.Add(TopRight);

            Triangles.Add(TopRight);
            Triangles.Add(BottomLeft);
            Triangles.Add(BottomRight);
        }
    }

    // Create the mesh section
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}

/*
void AProcMesh::GenerateMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FLinearColor> VertexColors; // Change FColor to FLinearColor
    TArray<FProcMeshTangent> Tangents;

    // Add three vertices for a simple triangle
    Vertices.Add(FVector(0.0f, 0.0f, 0.0f)); // Vertex 0
    Vertices.Add(FVector(0.0f, 100.0f, 0.0f)); // Vertex 1
    Vertices.Add(FVector(100.0f, 0.0f, 0.0f)); // Vertex 2

    // Define the triangle using indices
    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(2);

    // Normals for each vertex
    Normals.Add(FVector(0.0f, 0.0f, 1.0f)); // Normal for Vertex 0
    Normals.Add(FVector(0.0f, 0.0f, 1.0f)); // Normal for Vertex 1
    Normals.Add(FVector(0.0f, 0.0f, 1.0f)); // Normal for Vertex 2

    // UV mapping
    UV0.Add(FVector2D(0.0f, 0.0f)); // UV for Vertex 0
    UV0.Add(FVector2D(0.0f, 1.0f)); // UV for Vertex 1
    UV0.Add(FVector2D(1.0f, 0.0f)); // UV for Vertex 2

    // Vertex colors using FLinearColor
    VertexColors.Add(FLinearColor::Red);   // Color for Vertex 0
    VertexColors.Add(FLinearColor::Green); // Color for Vertex 1
    VertexColors.Add(FLinearColor::Blue);  // Color for Vertex 2

    // Tangents for each vertex (optional)
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f)); // Tangent for Vertex 0
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f)); // Tangent for Vertex 1
    Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f)); // Tangent for Vertex 2

    // Create the mesh section
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

    // Enable collision for this mesh
    ProceduralMesh->ContainsPhysicsTriMeshData(true);
}
*/
