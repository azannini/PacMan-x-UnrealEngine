// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazePawn.h"
#include "GhostPawn.generated.h"



UCLASS()
class PACMAN_API AGhostPawn : public AMazePawn
{
	GENERATED_BODY()
public:
	AGhostPawn();
	// need to override this function from MazePawn to handle ghost movement
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnNodeReached() override;

	// override per gestire comportamento dei fantasmi nel tunnel
	virtual void HandleMovement() override;

public:

	UFUNCTION()
		APacManPawn* GetPlayer() const;

	UFUNCTION()
		void SetSpeed(float Speed);

	UFUNCTION()
		virtual APacManNode* GetPlayerRelativeTarget();
	
	UPROPERTY(VisibleAnywhere)
		class APacManPawn* Player;

	UPROPERTY(EditAnywhere)
		float TunnelSpeed;


	virtual void SetGhostTarget();

	//Mesh components
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshLeftEye;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshRightEye;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshLeftEyePupil;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshRightEyePupil;

	//Mesh change for frightened mode
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshBlue;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshWhite;

	//Function that handle frightened state 

	void GhostFrightenedState();

	//Spped functions

	void FrightenedSpeed();
	void ChaseScatterSpeed();

	//Function to invert direction;

	void InvertDirection();

	//Function and variables to make ghosts Mesh blue and white 

	void MeshHandler();
	void MeshWhite();
	void MeshBlue();
	void ResetMesh();

	FTimerHandle WhiteMeshTimer;
	FTimerHandle BlueMeshTimer;
	float WhiteTime;
	float BlueTime;
	float FlashCounter;

	//Function to generate random target

	void SetFrightenedStateTarget();

	FRandomStream Stream;

	FVector DirCasualVector;

	FVector2D RandomNodeCord;

	APacManNode* RandomNode;


	//Variable to see if ghost is eaten and handel respawn

	bool IsEaten;

	void RespawnGhost(FVector2D RespawnNode);

	void ExitRespawnState();

	FTimerHandle GhostRestartTimer;

	void StopMovement();
	void MoveAgain();

	//Spawn state

	bool IsSpawnState;

	bool IsRespawn;

	void ExitSpawnStateGhost();

	//State where PacMan is eaten 

	void GhostRestart(FVector2D SpawnLocation);
};



