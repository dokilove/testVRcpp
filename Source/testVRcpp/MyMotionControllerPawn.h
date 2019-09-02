// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyMotionControllerPawn.generated.h"

UCLASS()
class TESTVRCPP_API AMyMotionControllerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyMotionControllerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class USceneComponent* VROrigin;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Default")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
		float DefaultPlayerHeight = 180.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
		bool UseControllerRollToRotate = false;
	
	class UClass* Controller;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
		class AMyMotionController* LeftController;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
		class AMyMotionController* RightController;
};
