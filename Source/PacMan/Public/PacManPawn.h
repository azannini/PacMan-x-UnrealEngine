// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazePawn.h"
#include "MazeGenerator.h"
#include "PacManPlayerController.h"
#include "PacManNode.h"
#include "GhostBaseNode.h"
#include "GhostPawn.h"
#include "PowerPoint.h"
#include "FruitActor.h"
#include "PacManPawn.generated.h"



UCLASS()
class PACMAN_API APacManPawn : public AMazePawn
{
	GENERATED_BODY()

	APacManPawn();
	// override to MazePawn functions (left empty on purpose)
	virtual void Tick(float DeltaTime) override;
	virtual void SetVerticalInput(float AxisValue) override;
	virtual void SetHorizontalInput(float AxisValue) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AGhostBaseNode> GhostClass;

	AGhostBaseNode* GhostNode;

protected:
	virtual void BeginPlay() override;
	virtual void HandleMovement() override;
	virtual void SetTargetNode(APacManNode* Node) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void SetNodeGeneric(const FVector Dir) override;
	virtual void SetNextNodeByDir(FVector InputDir, bool ForceLast) override;
 
public:
    
	void Eat(AGhostPawn* Ghost);

	void RespawnPacMan();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (AllowPrivateAccess = true))
		float PointCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (AllowPrivateAccess = true))
		float PointCounterUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = (AllowPrivateAccess = true))
		float LifeCounter;

	////////////////////////////////////////
	//Obj and function to handle fruit spawn

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AFruitActor> FruitActor;

	// reference to a Fruit object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AFruitActor* Fruit;

	void SpawnFruit();

	bool Spawnable;

	float minX;
	float maxX;
	float minY;
	float maxY;
};
