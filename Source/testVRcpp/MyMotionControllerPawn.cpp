// Fill out your copyright notice in the D escription page of Project Settings.


#include "MyMotionControllerPawn.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "MyMotionController.h"
#include "Engine/World.h"

// Sets default values
AMyMotionControllerPawn::AMyMotionControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(DefaultSceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);
}

// Called when the game starts or when spawned
void AMyMotionControllerPawn::BeginPlay()
{
	Super::BeginPlay();

	FName hmdName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

	/*UE_LOG(LogTemp, Warning, TEXT("HMD Name %s"), *hmdName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("HMD Name %d"), hmdName.GetNumber());*/

	if (hmdName.Compare(TEXT("SteamVR")) == 0
		|| hmdName.Compare(TEXT("OculusHMD")) == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("This is SteamVR or OculusHMD"));
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
	else if (hmdName.Compare(TEXT("PSVR")) == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("This is PSVR"));
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);

		VROrigin->AddLocalOffset(FVector(0.0f, 0.0f, DefaultPlayerHeight));
		UseControllerRollToRotate = true;
	}		
	   
	LeftController = GetWorld()->SpawnActor<AMyMotionController>(FVector(0, 0, 0), FRotator(0, 0, 0));
	LeftController->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	LeftController->SetOwner(this);
	LeftController->Hand = EControllerHand::Left;

	RightController = GetWorld()->SpawnActor<AMyMotionController>(FVector(0, 0, 0), FRotator(0, 0, 0));
	RightController->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	RightController->SetOwner(this);
	RightController->Hand = EControllerHand::Right;

	FAttachmentTransformRules ControllerAttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftController->AttachToComponent(VROrigin, ControllerAttachmentRules);
	RightController->AttachToComponent(VROrigin, ControllerAttachmentRules);
}

// Called every frame
void AMyMotionControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyMotionControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

