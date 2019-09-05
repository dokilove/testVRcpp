// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMotionController.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "../Plugins/Runtime/Steam/SteamVR/Source/SteamVR/Classes/SteamVRChaperoneComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/NavigationSystem/Public/NavAreas/NavArea_Obstacle.h"

// Sets default values
AMyMotionController::AMyMotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->SetupAttachment(Scene);

	
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(MotionController);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Hand(TEXT("SkeletalMesh'/Game/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right.MannequinHand_Right'"));
	if (SK_Hand.Succeeded())
	{
		HandMesh->SetSkeletalMesh(SK_Hand.Object);
	}

	HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	FStringClassReference RightHand_Anim_Ref(TEXT("AnimBlueprint'/Game/VirtualReality/Mannequin/Animations/RightHand_AnimBP.RightHand_AnimBP_C'"));
	if (UClass* RightHand_Anim_Class = RightHand_Anim_Ref.TryLoadClass<UAnimInstance>())
	{
		HandMesh->SetAnimInstanceClass(RightHand_Anim_Class);
	}

	ArcDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ArcDirection"));
	ArcDirection->SetupAttachment(HandMesh);

	ArcSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ArcSpline"));
	ArcSpline->SetupAttachment(HandMesh);

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
	GrabSphere->SetupAttachment(HandMesh);
	
	GrabSphere->SetRelativeLocation(FVector(14.0, 0.22, 1.5));
	GrabSphere->SetSphereRadius(10.0f);
	//GrabSphere->AreaClass = new TSubclassOf<UNavArea_Obstacle>();

	ArcEndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcEndPoint"));
	ArcEndPoint->SetupAttachment(MotionController);

	TeleportCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportCylinder"));
	TeleportCylinder->SetupAttachment(MotionController);
	
	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
	Ring->SetupAttachment(TeleportCylinder);
	
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(TeleportCylinder);
	
	RoomScaleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomScaleMesh"));
	RoomScaleMesh->SetupAttachment(Arrow);

	SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVRChaperone"));
}

void AMyMotionController::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	//Update the motion controller Hand
	MotionController->MotionSource = UEnum::GetValueAsName(Hand);

	UE_LOG(LogClass, Warning, TEXT("Motion source %s"), *(MotionController->MotionSource).ToString());
}

// Called when the game starts or when spawned
void AMyMotionController::BeginPlay()
{
	Super::BeginPlay();

	switch (Hand)
	{
	case EControllerHand::Left:
		HandMesh->SetWorldScale3D(FVector(1, 1, -1));
		break;
	default:
		break;
	}
}

// Called every frame
void AMyMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyMotionController::GrabActor()
{
}

void AMyMotionController::ReleaseActor()
{
}

void AMyMotionController::GetActorNearHand(UActorComponent * NearestMesh)
{
	UActorComponent* NearestOverlappingActor = nullptr;
	float NearestOverlap = 10000.0f;

	TArray<AActor*> OverlappedActors;
	GrabSphere->GetOverlappingActors(OverlappedActors);
	
	FVector SphereLocation = GrabSphere->GetComponentLocation();

	for (int i = 0; i < OverlappedActors.Max(); ++i)
	{
		FVector DistanceVector = OverlappedActors[i]->GetActorLocation() - SphereLocation;
	}
}

