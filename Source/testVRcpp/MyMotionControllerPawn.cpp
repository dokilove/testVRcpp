// Fill out your copyright notice in the D escription page of Project Settings.


#include "MyMotionControllerPawn.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AMyMotionControllerPawn::AMyMotionControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);

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
}

// Called when the game starts or when spawned
void AMyMotionControllerPawn::BeginPlay()
{
	Super::BeginPlay();
	
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

