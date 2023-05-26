// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacManGameMode.h"



AInky::AInky()
{
}


void AInky::SetGhostTarget()
{
	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	FVector2D Tmp_Coord = FVector2D(0, 0);

	FVector BlinkyPosition = FVector(0, 0, 0);

	FVector2D BlinkyCoord = FVector2D(0, 0);

	FVector2D TargetCoord = FVector2D(0, 0);

	//Inky needs PacmMan and Blinky current node.We need to get two node ahead of PacMan,then we take blinky position and draw a vector towards pacman position and then double it.
	if (IsSpawnState)
	{
		// SpawnState ... Target--> InkySpawn
		Target = *(MazeGen->TileMap.Find(InkySpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
	}
	else if (IsEaten)
	{
		// Eaten ... Target--> InkySpawn
		Target = *(MazeGen->TileMap.Find(InkySpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
		RespawnGhost(InkySpawn);
		
		if (CurrentGridCoords == Target->GetNodePosition())
		{
			
			IsEaten = false;
			//Get out of respwan state
			ExitRespawnState();
		
		}
		
	}
	else if (GameMode->EStates != Frightened)
	{
		if (GameMode->EStates == Chase ) {

			//Get Blinky position 3D
			BlinkyPosition = GameMode->Blinky->GetActorLocation();
			BlinkyCoord = MazeGen->GetFVector2DfromFVector(BlinkyPosition);

			if (Player->GetLastValidDirection() == FVector(0, 1, 0))
			{
				Tmp_Coord = (Player->GetLastNodeCoords() + FVector2D(2, 0));
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);

				//Now i calculate the vector that connects blinky's vector and pacman offset vector and double it

				TargetCoord = FVector2D(((Tmp_Coord.X - BlinkyCoord.X) * 2 + BlinkyCoord.X),((Tmp_Coord.Y - BlinkyCoord.Y) * 2 + BlinkyCoord.Y));
				TargetCoord.X = FMath::Clamp(TargetCoord.X, 0, 36);
				TargetCoord.Y = FMath::Clamp(TargetCoord.Y, 0, 27);

				Target = *(MazeGen->TileMap.Find(TargetCoord));
			}
			else if (Player->GetLastValidDirection() == FVector(0, -1, 0))
			{
				Tmp_Coord = Player->GetLastNodeCoords() + FVector2D(-2, 0);
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);
		         
				//Now i calculate the vector that connects blinky's vector and pacman offset vector and double it
				TargetCoord = FVector2D(((Tmp_Coord.X - BlinkyCoord.X) * 2 + BlinkyCoord.X), ((Tmp_Coord.Y - BlinkyCoord.Y) * 2 + BlinkyCoord.Y));
				TargetCoord.X = FMath::Clamp(TargetCoord.X, 0, 36);
				TargetCoord.Y = FMath::Clamp(TargetCoord.Y, 0, 27);

				Target = *(MazeGen->TileMap.Find(TargetCoord));

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f,%f"),(Tmp_Coord.X, Tmp_Coord.Y)));
			}
			else if (Player->GetLastValidDirection() == FVector(1, 0, 0))
			{
				Tmp_Coord = Player->GetLastNodeCoords() + FVector2D(0, 2);
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);
		
				//Now i calculate the vector that connects blinky's vector and pacman offset vector and double it

				TargetCoord = FVector2D(((Tmp_Coord.X - BlinkyCoord.X) * 2 + BlinkyCoord.X), ((Tmp_Coord.Y - BlinkyCoord.Y) * 2 + BlinkyCoord.Y));
				TargetCoord.X = FMath::Clamp(TargetCoord.X, 0, 36);
				TargetCoord.Y = FMath::Clamp(TargetCoord.Y, 0, 27);

				Target = *(MazeGen->TileMap.Find(TargetCoord));

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f,%f"), (Tmp_Coord.X, Tmp_Coord.Y)));
			}
			else if (Player->GetLastValidDirection() == FVector(-1, 0, 0))
			{
				Tmp_Coord = Player->GetLastNodeCoords() + FVector2D(0, -2);
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);
				
				//Now i calculate the vector that connects blinky's vector and pacman offset vector a

				TargetCoord = FVector2D(((Tmp_Coord.X - BlinkyCoord.X) * 2 + BlinkyCoord.X), ((Tmp_Coord.Y - BlinkyCoord.Y) * 2 + BlinkyCoord.Y));
				TargetCoord.X = FMath::Clamp(TargetCoord.X, 0, 36);
				TargetCoord.Y = FMath::Clamp(TargetCoord.Y, 0, 27);

				Target = *(MazeGen->TileMap.Find(TargetCoord));

				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f,%f"), (Tmp_Coord.X, Tmp_Coord.Y)));
			}

			else
			{
				Target = GetPlayerRelativeTarget();

				if (!Target)
				{
					Target = GetPlayer()->GetLastNode();
				}
			}

		}
		else
		{
			Target = InkyScatterNode;
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

void AInky::BeginPlay()
{
	AGhostPawn::BeginPlay();

	CanMove = true;

	InkyScatterNodeCoord = FVector2D(2, 27);

	InkyScatterNode = *(MazeGen->TileMap.Find(InkyScatterNodeCoord));
}
