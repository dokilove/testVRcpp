// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRightHandAnimInstance.h"
#include "MyMotionController.generated.h"

UCLASS()
class TESTVRCPP_API AMyMotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyMotionController();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UArrowComponent* ArcDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class USplineComponent* ArcSpline;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class USphereComponent* GrabSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* ArcEndPoint;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* TeleportCylinder;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* Ring;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* Arrow;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* RoomScaleMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class USteamVRChaperoneComponent* SteamVRChaperone;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
		EControllerHand Hand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn = "true"))
		bool bWantsToGrip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn = "true"))
		class AActor* AttachedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn = "true"))
		EGripEnum GripState;

	UFUNCTION(BlueprintCallable, Category = "Grabbing")
		void GrabActor();

	UFUNCTION(BlueprintCallable, Category = "Grabbing")
		void ReleaseActor();

	UFUNCTION(BlueprintCallable, Category = "Grabbing")
		class AActor* GetActorNearHand();

	UFUNCTION(BlueprintCallable)
		void RumbleController(float Intensity);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "HapticEffect")
		class UHapticFeedbackEffect_Base* MotionControllerHapticFeedbackEffect;
};
