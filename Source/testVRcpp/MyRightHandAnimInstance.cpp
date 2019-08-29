// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRightHandAnimInstance.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

void UMyRightHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
		
	float fGripState = (float)(GripState) / 2.0f;
	Grip = FMath::FInterpConstantTo(Grip, fGripState, DeltaSeconds, InterpSpeed);
}