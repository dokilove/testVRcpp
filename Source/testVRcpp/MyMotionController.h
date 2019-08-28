// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyMotionController.generated.h"

UCLASS()
class TESTVRCPP_API AMyMotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyMotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class UArrowComponent* ArcDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class USplineComponent* ArcSpline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class USphereComponent* GrabSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* ArcEndPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* TeleportCylinder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* Ring;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* Arrow;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class UStaticMeshComponent* RoomScaleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class USteamVRChaperoneComponent* SteamVRChaperone;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
		EControllerHand Hand;
};
