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
#include "Materials/MaterialInstanceDynamic.h"

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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ArcEndPoint(TEXT("StaticMesh'/Game/Resources/Sphere1.Sphere1'"));
	if (SM_ArcEndPoint.Succeeded())
	{
		ArcEndPoint->SetStaticMesh(SM_ArcEndPoint.Object);
		static ConstructorHelpers::FObjectFinder<UMaterial> M_ArcEndPoint(TEXT("Material'/Game/Materials/M_ArcEndpoint.M_ArcEndpoint'"));
		if (M_ArcEndPoint.Succeeded())
		{
			UMaterial* M = (UMaterial*)M_ArcEndPoint.Object;
			ArcEndPoint->SetMaterial(0, M);
		}
	}
	ArcEndPoint->SetRelativeScale3D(FVector(0.15, 0.15, 0.15));
	ArcEndPoint->SetGenerateOverlapEvents(false);
	ArcEndPoint->SetCollisionProfileName("NoCollision");
	ArcEndPoint->SetVisibility(false);

	TeleportCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportCylinder"));
	TeleportCylinder->SetupAttachment(MotionController);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TeleportCylinder(TEXT("StaticMesh'/Game/Resources/Cylinder1.Cylinder1'"));
	if (SM_TeleportCylinder.Succeeded())
	{
		TeleportCylinder->SetStaticMesh(SM_TeleportCylinder.Object);
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_TeleportCylinder(TEXT("MaterialInstanceConstant'/Game/Materials/MI_TeleportCylinderPreview.MI_TeleportCylinderPreview'"));
		if (MI_TeleportCylinder.Succeeded())
		{
			UMaterialInstance* MI = (UMaterialInstance*)MI_TeleportCylinder.Object;
			TeleportCylinder->SetMaterial(0, MI);
		}
	}
	TeleportCylinder->SetRelativeScale3D(FVector(0.75, 0.75, 1.0));
	TeleportCylinder->SetCollisionProfileName("NoCollision");	   

	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
	Ring->SetupAttachment(TeleportCylinder); 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Ring(TEXT("StaticMesh'/Game/VirtualReality/Meshes/SM_FatCylinder.SM_FatCylinder'"));
	if (SM_Ring.Succeeded())
	{
		Ring->SetStaticMesh(SM_Ring.Object);
		static ConstructorHelpers::FObjectFinder<UMaterial> M_Ring(TEXT("Material'/Game/Materials/M_ArcEndpoint.M_ArcEndpoint'"));
		if (M_Ring.Succeeded())
		{
			UMaterial* M = (UMaterial*)M_Ring.Object;
			Ring->SetMaterial(0, M);
		}
	}
	Ring->SetRelativeScale3D(FVector(0.5,0.5,0.15));
	
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(TeleportCylinder);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Arrow(TEXT("StaticMesh'/Game/VirtualReality/Meshes/BeaconDirection.BeaconDirection'"));
	if (SM_Arrow.Succeeded())
	{
		Arrow->SetStaticMesh(SM_Arrow.Object);
		static ConstructorHelpers::FObjectFinder<UMaterial> M_Arrow(TEXT("Material'/Game/Materials/M_ArcEndpoint.M_ArcEndpoint'"));
		if (M_Arrow.Succeeded())
		{
			UMaterial* M = (UMaterial*)M_Arrow.Object;
			Arrow->SetMaterial(0, M);
		}
	}
	Arrow->SetCollisionProfileName("NoCollision");
	
	RoomScaleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomScaleMesh"));
	RoomScaleMesh->SetupAttachment(Arrow);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_RoomScaleMesh(TEXT("StaticMesh'/Game/VirtualReality/Meshes/1x1_cube.1x1_cube'"));
	if (SM_RoomScaleMesh.Succeeded())
	{
		RoomScaleMesh->SetStaticMesh(SM_RoomScaleMesh.Object);
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_RoomScaleMesh(TEXT("MaterialInstanceConstant'/Game/Materials/MI_ChaperoneOutline.MI_ChaperoneOutline'"));
		if (MI_RoomScaleMesh.Succeeded())
		{
			UMaterialInstance* MI = (UMaterialInstance*)MI_RoomScaleMesh.Object;
			RoomScaleMesh->SetMaterial(0, MI);
		}
	}
	RoomScaleMesh->SetGenerateOverlapEvents(false);
	RoomScaleMesh->SetCollisionProfileName("NoCollision");

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

