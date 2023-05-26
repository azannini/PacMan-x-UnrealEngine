// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Blinky.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API ABlinky : public AGhostPawn
{
	GENERATED_BODY()
	
public:

	ABlinky();

	// need to call for override on Tick bcs of ElroyEnter()
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetGhostTarget() override;
	virtual APacManNode* GetPlayerRelativeTarget() override;

	void ElroyEnter();

	//Bool to check if is Elroy
	UPROPERTY(EditAnywhere)
		bool IsElroy;

	//needs to be public for PacManGameode to access it
	FVector2D BlinkySpawn = FVector2D(20, 16);
private:
	//Blinky Scatter Coordinates both in vector2D and APacManNode
	FVector2D BlinkyScatterNodeCoord;
	APacManNode* BlinkyScatterNode;
};
