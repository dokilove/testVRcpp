// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyRightHandAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EGrabEnum : uint8
{
	Open		UMETA(DisplayName="Open"),
	CanGrab		UMETA(DisplayName="CanGrab"),
	Grab		UMETA(DisplayName="Grab")
};

/**
 * 
 */
UCLASS()
class TESTVRCPP_API UMyRightHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGrabEnum GripState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InterpSpeed = 7.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Grip;

};
