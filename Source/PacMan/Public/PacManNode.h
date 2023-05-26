// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacManNode.generated.h"

UENUM()
enum EIsWalkable {NotWalkable, Walkable};
 
UCLASS()
class PACMAN_API APacManNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APacManNode();

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EIsWalkable> EIsWalkable = Walkable;

	// 2D Coordinates of a node
	UPROPERTY(EditAnywhere)
		FVector2D NodeGridPosition;

	// 3D Coordinates of a node
	UPROPERTY(EditAnywhere)
		FVector NodeCoordinatesPosition;

	//--------------------------------------------------------------
	// variable used to slow down the speed of ghosts in the tunnel
	// default value = false -> TRUE only for the TeleportNodes
	bool isTeleport;
		
	// 2D coordinates setter
	void SetNodePosition(const double InX, const double InY);

	// 2D coordinates getter
	FVector2D GetNodePosition() const;

	// 3D coordinates getter
	FVector GetNodeCoordinates();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
