// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Clyde.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AClyde : public AGhostPawn
{
	GENERATED_BODY()
	
public:

	AClyde();
	
	// no need to call for override on Tick

	virtual void SetGhostTarget() override;

	virtual void BeginPlay() override;

	//needs to be public for PacManGameode to access it
	FVector2D ClydeSpawn = FVector2D(20,11);

private:
	// Clyde Scatter Coordinates both in vector2D and APacManNode
	FVector2D ClydeScatterNodeCoord;
	APacManNode* ClydeScatterNode;
};
