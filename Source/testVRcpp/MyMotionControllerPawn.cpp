// Fill out your copyright notice in the D escription page of Project Settings.


#include "MyMotionControllerPawn.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "MyMotionController.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

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

	/*static ConstructorHelpers::FObjectFinder<UBlueprint> BP_Controller(TEXT("Blueprint'/Game/Blueprints/BP_MotionController.BP_MotionController'"));
	if (BP_Controller.Succeeded())
	{
		Controller = Cast<UClass>(BP_Controller.Object->GeneratedClass);
	}*/
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
	   
	FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(1, 1, 1));
	
	LeftController = GetWorld()->SpawnActorDeferred<AMyMotionController>(ControllerBP, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (LeftController)
	{
		LeftController->Hand = EControllerHand::Left;
		UGameplayStatics::FinishSpawningActor(LeftController, SpawnTransform);
	}

	RightController = GetWorld()->SpawnActorDeferred<AMyMotionController>(ControllerBP, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (RightController)
	{
		RightController->Hand = EControllerHand::Right;
		UGameplayStatics::FinishSpawningActor(RightController, SpawnTransform);
	}

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

	PlayerInputComponent->BindAction(TEXT("GrabRight"), EInputEvent::IE_Pressed, this, &AMyMotionControllerPawn::RightGrab);
	PlayerInputComponent->BindAction(TEXT("GrabRight"), EInputEvent::IE_Released, this, &AMyMotionControllerPawn::RightRealese);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), EInputEvent::IE_Pressed, this, &AMyMotionControllerPawn::LeftGrab);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), EInputEvent::IE_Released, this, &AMyMotionControllerPawn::LeftRealese);
}

void AMyMotionControllerPawn::RightGrab()
{
	RightController->GrabActor();
}

void AMyMotionControllerPawn::RightRealese()
{
	RightController->ReleaseActor();
}

void AMyMotionControllerPawn::LeftGrab()
{
	LeftController->GrabActor();
}

void AMyMotionControllerPawn::LeftRealese()
{
	LeftController->ReleaseActor();
}

