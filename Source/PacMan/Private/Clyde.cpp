// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"
#include "PacManGameMode.h"
#include "Math/Vector.h"

AClyde::AClyde()
{
}


void AClyde::SetGhostTarget()
{
	
	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	if (IsSpawnState)
	{
		// SpawnState ... Target--> ClydeSpawn
		Target = *(MazeGen->TileMap.Find(ClydeSpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
	}
	else if (IsEaten)
	{
		// Eaten ... Target--> ClydeSpawn
		Target = *(MazeGen->TileMap.Find(ClydeSpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
		RespawnGhost(ClydeSpawn);
		
		if (CurrentGridCoords == Target->GetNodePosition())
		{	
			IsEaten = false;
			//Get out of respwan state
			ExitRespawnState();
		}


	}

	else if (GameMode->EStates != Frightened)
	{
		if ((GameMode->EStates == Chase)) {
			//Calc euclide distance from Pacman ,if more than 8 tiles target Pacman, else target the scatter mode node
			if (FVector::Dist(this->GetActorLocation(), GetPlayer()->GetActorLocation()) > 850)
			{
				Target = GetPlayerRelativeTarget();

				if (!Target)
				{
					Target = GetPlayer()->GetLastNode();
				}
			}
			else
			{
				Target = ClydeScatterNode;
			}
		}
		else
		{
			Target = ClydeScatterNode;
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

void AClyde::BeginPlay()
{
	AGhostPawn::BeginPlay();

	CanMove = true;

	ClydeScatterNodeCoord = FVector2D(2, 0);

	ClydeScatterNode = *(MazeGen->TileMap.Find(ClydeScatterNodeCoord));
}
