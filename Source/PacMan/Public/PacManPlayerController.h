// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazePawn.h"
#include "GameFramework/PlayerController.h"
#include "PacManPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API APacManPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APacManPlayerController();
	void SetHorizontal(float Amount);
	void SetVertical(float Amount);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
