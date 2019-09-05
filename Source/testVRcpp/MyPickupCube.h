// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MyPickupActorInterface.h"
#include "MyPickupCube.generated.h"

/**
 * 
 */
UCLASS()
class TESTVRCPP_API AMyPickupCube : public AStaticMeshActor, public IMyPickupActorInterface
{
	GENERATED_BODY()
	
public:
	AMyPickupCube();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grab")
		void Pickup(class USceneComponent* AttachTo);
	void Pickup_Implementation(class USceneComponent* AttachTo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grab")
		void Drop();
	void Drop_Implementation();
};
