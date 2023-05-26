// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"
#include "PacManGameMode.h"

ABlinky::ABlinky()
{
	IsElroy = false;
}

APacManNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

void ABlinky::BeginPlay()
{
	AGhostPawn::BeginPlay();

	BlinkyScatterNodeCoord = FVector2D(35, 25);

	BlinkyScatterNode = *(MazeGen->TileMap.Find(BlinkyScatterNodeCoord));
}

void ABlinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Elroy enabled when -20 or -10 points to total (total 234)
	// Called twice bcs at 224 points speed is set to 85% of standard speed, instead of 80%
	if (Player->PointCounter == 214  || Player->PointCounter == 224)
	{
		ElroyEnter();
	}
}

void ABlinky::SetGhostTarget()
{
	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	if (IsSpawnState)
	{
		// SpawnState ... Target--> BlinkySpawn
		Target = *(MazeGen->TileMap.Find(BlinkySpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
	}
	else if (IsEaten)
	{
		// Eaten ... Target--> BlinkySpawn
		Target = *(MazeGen->TileMap.Find(BlinkySpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
		RespawnGhost(BlinkySpawn);
		
		if (CurrentGridCoords == Target->GetNodePosition())
		{
			//When respawn is reached reset the ghost 
			
			IsEaten = false;
			//Get out of respwan state
			ExitRespawnState();
			
		}

	}

	else if (GameMode->EStates != Frightened)
	{
		if ((GameMode->EStates == Chase || IsElroy)) {

			Target = GetPlayerRelativeTarget();

			if (!Target)
			{
				Target = GetPlayer()->GetLastNode();
			}
		}
		else
		{
			Target = BlinkyScatterNode;
		}

		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));

	}

	else
	{
		SetFrightenedStateTarget();
		PossibleNode = TargetNode;
	}

	if (PossibleNode)
	{
		TargetNode = PossibleNode; 
		NextNode = PossibleNode;
		LastValidInputDirection = MazeGen->GetFVectorFromFVector2D(PossibleNode->GetNodePosition() - this->GetLastNodeCoords());
	}
}


void ABlinky::ElroyEnter()
{
	// When Blinky is Elroy he set his speed to the same as Pacman the first time 
	// The second time Elroy goes faster than PacMan

	IsElroy = true;

	if (Player->PointCounter == 214)
	{
	    CurrentMovementSpeed = (StandardSpeed / 100) * 80;
	}
	else
	{
	    CurrentMovementSpeed = (StandardSpeed / 100) * 85;
	}

}


