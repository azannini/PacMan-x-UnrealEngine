// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeGenerator.h"
#include "Blinky.h"
#include "Inky.h"
#include "Pinky.h"
#include "Clyde.h"
#include "PacManPawn.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameMode.generated.h"


//Game states

UENUM()
enum EStates { Chase, Scatter, Frightened, SpawnState};


UCLASS()
class PACMAN_API APacManGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	//Istanza di un oggetto MazeGenerator
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AMazeGenerator> MazeGeneratorClass;

	//Puntatore ad un oggetto MazeGenerator
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AMazeGenerator* GMaze;

	APacManGameMode();

	//State machine objects

	UPROPERTY(EditAnywhere)
		TSubclassOf<APinky> PinkyClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABlinky> BlinkyClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AInky> InkyClass;


	UPROPERTY(EditAnywhere)
		TSubclassOf<AClyde> ClydeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APacManPawn> PacManClass;


	UPROPERTY(EditAnywhere)
		APinky* Pinky;

	UPROPERTY(EditAnywhere)
		 ABlinky* Blinky;

	UPROPERTY(EditAnywhere)
		 AInky* Inky;

	UPROPERTY(EditAnywhere)
		AClyde* Clyde;

	UPROPERTY(EditAnywhere)
		 APacManPawn* PacMan;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EStates> EStates;

	//Int to save last state when entered Frightened Mode

	UPROPERTY(EditAnywhere)
	int LastState;

    //Counter to see how many times Scatter state has been activated

	UPROPERTY(EditAnywhere)
		int Scatter_Counter;

	//Counter to see how many times Chase state has been activated
	
	UPROPERTY(EditAnywhere)
		int Chase_Counter;

	//Timers

	FTimerHandle FrightnedTimer;
	
	FTimerHandle ChaseTimer;

	FTimerHandle ScatterTimer;

	FTimerHandle SpawnStateTimer;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//State Functions

	void EnterFrightenedMode();

	void FrightenedExit();

	void EnterChaseMode();

	void ChaseExit();

	void EnterScatterMode();

	void ScatterExit();

	void Respawn();

	void EnterSpawnState();

	void ExitSpawnState();
};
