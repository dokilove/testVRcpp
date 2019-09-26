// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPickupCube.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

AMyPickupCube::AMyPickupCube()
{
	this->bCanBeDamaged = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("StaticMesh'/Game/Resources/Cube1.Cube1'"));
	if (SM_Body.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(SM_Body.Object);
	}
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);

	
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);		
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	GetStaticMeshComponent()->SetCollisionProfileName("PhysicsActor");
	GetStaticMeshComponent()->SetUseCCD(true);

}

void AMyPickupCube::Pickup_Implementation(USceneComponent * AttachTo)
{
	GetStaticMeshComponent()->SetSimulatePhysics(false);

	FAttachmentTransformRules ControllerAttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	RootComponent->AttachToComponent(AttachTo, ControllerAttachmentRules);

}

void AMyPickupCube::Drop_Implementation()
{
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	FDetachmentTransformRules ControllerDetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
	DetachFromActor(ControllerDetachmentRules);
}
