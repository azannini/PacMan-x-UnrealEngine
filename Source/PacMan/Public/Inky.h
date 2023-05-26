// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Inky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API AInky : public AGhostPawn
{
	GENERATED_BODY()

public:	

	AInky();

	virtual void SetGhostTarget() override;

	virtual void BeginPlay() override;
	
	//needs to be public for PacManGameode to access it
	FVector2D InkySpawn = FVector2D(20, 18);

private:
	//Inky Scatter Coordinates both in vector2D and APacManNode
	FVector2D InkyScatterNodeCoord;
	APacManNode* InkyScatterNode;
};
