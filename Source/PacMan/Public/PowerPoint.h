// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PointActor.h"
#include "PowerPoint.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APowerPoint : public APointActor
{
	GENERATED_BODY()

public:
	FTimerHandle PointFlash;

	void FlashPoint();
	void ExitFlashPoint();
	bool IsEaten;

	virtual void BeginPlay() override;
	
};
