// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Pinky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APinky : public AGhostPawn
{
	GENERATED_BODY()

public:

	APinky();

	// no need to call for override on Tick

	virtual void SetGhostTarget() override;

	virtual void BeginPlay() override;

	FVector2D PinkySpawn = FVector2D(20, 9);
private:

	FVector2D PinkyScatterNodeCoord;

	APacManNode* PinkyScatterNode;
};
