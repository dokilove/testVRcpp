// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyPickupActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyPickupActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTVRCPP_API IMyPickupActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grab")
		void Pickup(class USceneComponent* AttachTo);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grab")
		void Drop();

};
