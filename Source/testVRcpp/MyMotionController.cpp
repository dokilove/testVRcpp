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
#include "Kismet/KismetMathLibrary.h"
#include "MyPickupActorInterface.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Haptics/HapticFeedbackEffect_Base.h"
#include "Animation/AnimInstance.h"
#include "MyRightHandAnimInstance.h"

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

	HandMesh->SetRelativeLocation(FVector(-12, -0.02, -1.7));
	HandMesh->SetRelativeRotation(FRotator(0, 0, 90));

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
	ArcDirection->SetRelativeLocation(FVector(14.17, 0.85, -4.3));
	ArcDirection->ArrowSize = 0.2f;

	ArcSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ArcSpline"));
	ArcSpline->SetupAttachment(HandMesh);
	ArcSpline->SetRelativeLocation(FVector(12.53, -1.75, 2.55));

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
	
	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> MotionControllerHaptics(TEXT("HapticFeedbackEffect_Curve'/Game/Blueprints/MotionControllerHaptics.MotionControllerHaptics'"));
	if (MotionControllerHaptics.Succeeded())
	{
		MotionControllerHapticFeedbackEffect = MotionControllerHaptics.Object;
	}
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

	// Update Animation of Hand
	if (AttachedActor != nullptr || bWantsToGrip)
	{
		GripState = EGripEnum::Grab;
	}
	else
	{
		if (GetActorNearHand() != nullptr)
		{
			GripState = EGripEnum::CanGrab;
		}
		else
		{
			if (bWantsToGrip)
			{
				GripState = EGripEnum::Grab;
			}
			else
			{
				GripState = EGripEnum::Open;
			}
		}
	}

	// Update the Animation state of hand mesh
	UMyRightHandAnimInstance* HandAnimInstance = Cast<UMyRightHandAnimInstance>(HandMesh->GetAnimInstance());
	if (HandAnimInstance)
	{
		HandAnimInstance->GripState = GripState;
	}

}

void AMyMotionController::GrabActor()
{
	bWantsToGrip = true;
	AActor* NearestActor = GetActorNearHand();
	if (NearestActor && NearestActor->IsValidLowLevel())
	{
		AttachedActor = NearestActor;
		if (AttachedActor->Implements<UMyPickupActorInterface>())
		{
			IMyPickupActorInterface::Execute_Pickup(AttachedActor, MotionController);			
			RumbleController(0.7f);
		}
	}
}

void AMyMotionController::ReleaseActor()
{
	bWantsToGrip = false;
	if (AttachedActor && AttachedActor->IsValidLowLevel())
	{
		if (AttachedActor->GetRootComponent()->GetAttachParent() == MotionController)
		{
			if (AttachedActor->Implements<UMyPickupActorInterface>())
			{
				IMyPickupActorInterface::Execute_Drop(AttachedActor);
				RumbleController(0.2f);
			}
		}

		AttachedActor = nullptr;
	}
}

class AActor* AMyMotionController::GetActorNearHand()
{
	AActor* NearestOverlappingActor = nullptr;
	float NearestOverlap = 10000.0f;

	TArray<AActor*> OverlappedActors;
	GrabSphere->GetOverlappingActors(OverlappedActors);
	
	FVector SphereLocation = GrabSphere->GetComponentLocation();

	// Assertion failed: (Index >= 0) & (Index < ArrayNum) 에러가 뜸
	/*for (int i = 0; i < OverlappedActors.Max(); ++i)
	{
		float Distance = (OverlappedActors[i]->GetActorLocation() - SphereLocation).Size();
		if (Distance < NearestOverlap)
		{
			NearestOverlappingActor = OverlappedActors[i];
			NearestOverlap = Distance;
		}
	}*/

	for (auto OverlappedActor : OverlappedActors)
	{
		if (OverlappedActor && OverlappedActor->IsValidLowLevel())
		{
			float Distance = (OverlappedActor->GetActorLocation() - SphereLocation).Size();
			if (Distance < NearestOverlap)
			{
				NearestOverlappingActor = OverlappedActor;
				NearestOverlap = Distance;
			}
		}
	}

	return NearestOverlappingActor;
}

void AMyMotionController::RumbleController(float Intensity)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->IsValidLowLevel())
	{
		PC->PlayHapticEffect(MotionControllerHapticFeedbackEffect, Hand, Intensity);
	}
}

