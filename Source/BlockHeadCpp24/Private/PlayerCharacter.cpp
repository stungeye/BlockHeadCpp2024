// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"
#include "Logging/StructuredLog.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Obstacle.h"
#include "EndPoint.h"

using UEnhancedInputSys = UEnhancedInputLocalPlayerSubsystem;
using UEnhancedInputComp = UEnhancedInputComponent;

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetSimulatePhysics(true);
	Cube->SetNotifyRigidBodyCollision(true); // Generate Hit Events
	RootComponent = Cube;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Cube);

	SpringArm->bEnableCameraLag = true; // TODO: Play test this
	SpringArm->SetRelativeRotation(FRotator{0.0f, 0.0f, -20.0f}); // Pitch (X), Yaw (Z), Roll (Y)
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController{ Cast<APlayerController>(GetController()) }) {
		const ULocalPlayer* LocalPlayer{ PlayerController->GetLocalPlayer() };
		if (UEnhancedInputSys* SubSystem{ ULocalPlayer::GetSubsystem<UEnhancedInputSys>(LocalPlayer) }) {
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// Bind my dynamic delegates for on hit and on component overlap:
	if (Cube) {
		Cube->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnHit);
		Cube->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);
	}
}

void APlayerCharacter::MoveRightLeft(const FInputActionValue& Value)
{
	const float MovementAxis{ Value.Get<float>() };
	
	if (!bLevelEnded) {
		const FVector CubeForce(0.0, MovementAxis * SideForce, 0.0);
		Cube->AddForce(CubeForce, NAME_None, true);
	}
}

void APlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(AObstacle::StaticClass())) {
		UE_LOGFMT(LogTemp, Warning, "We hit an obstacle! BOOM BOOM BOOM!");
	}
}

void APlayerCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEndPoint::StaticClass())) {
		bLevelEnded = true;
		UE_LOGFMT(LogTemp, Warning, "Reached the end point!");
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bLevelEnded) {
		const FVector CubeForce(ForwardForce, 0.0, 0.0);
		Cube->AddForce(CubeForce, NAME_None, true);
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComp* EnhancedComponent{ CastChecked<UEnhancedInputComp>(PlayerInputComponent) }) {
		EnhancedComponent->BindAction(MoveRightLeftInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRightLeft);
	}
}

