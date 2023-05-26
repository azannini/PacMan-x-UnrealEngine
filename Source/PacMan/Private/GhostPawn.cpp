// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPawn.h"
#include "MazePawn.h"
#include "PacManPawn.h"
#include "PacManGameMode.h"
#include "MazeGenerator.h"
#include <Perception/AIPerceptionComponent.h>
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

AGhostPawn::AGhostPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh eyes
	StaticMeshLeftEye = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left eye"));
	StaticMeshRightEye = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right eye"));
	StaticMeshLeftEyePupil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left eye Pupil"));
	StaticMeshRightEyePupil = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right eye Pupil"));

	//Mesh to handle frightened state 
	StaticMeshBlue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blue Mesh"));
	StaticMeshWhite = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("White Mesh"));

	StaticMeshLeftEyePupil->SetupAttachment(Collider);
	StaticMeshRightEyePupil->SetupAttachment(Collider);
	StaticMeshLeftEye->SetupAttachment(Collider);
	StaticMeshRightEye->SetupAttachment(Collider);
	StaticMeshBlue->SetupAttachment(Collider);
	StaticMeshWhite->SetupAttachment(Collider);

	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	//Time in blue and white 
	WhiteTime = 1.0;
	BlueTime = 1.0;
	FlashCounter = 0;

	//Initialize Tunnel Speed (StandardSpeed = 800.0f)
	TunnelSpeed = StandardSpeed * 0.45;

	IsEaten = false;
	IsSpawnState = false;
	IsRespawn = false;
}

void AGhostPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	MazeGen = GameMode->GMaze;
	LastNode = MazeGen->TileMap[StartNode];
	Player = Cast<APacManPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacManPawn::StaticClass()));

}

void AGhostPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	const auto PacMan = Cast<APacManPawn>(OtherActor);
	if (PacMan)
	{
		//When in Frightened state eat the Ghost
		if (GameMode->EStates == Frightened)
		{
			PacMan->Eat(this);
			//PacMan->PointCounterUI += 200;

			APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
			APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));

			EnterNode1->EIsWalkable = NotWalkable;
			EnterNode2->EIsWalkable = NotWalkable;
		}
		else 
		{   
			//If not in Frightened mode Ghost kill Pacman
			GameMode->Respawn();
			
		}
	}
}

void AGhostPawn::OnNodeReached()
{
	Super::OnNodeReached();
}



void AGhostPawn::HandleMovement()
{
	MoveToCurrentTargetNode();
	if (TargetNode == nullptr)
	{
		if (NextNode != nullptr)
		{
			SetTargetNode(NextNode);
			SetNextNode(nullptr);
			// funzione di interpolazione che fa muovere il pawn verso una nuova posizione data la posizione corrente
			const FVector2D StartVector = MazeGen->GetFVector2DfromFVector(GetActorLocation());
			const FVector2D EndVector = MazeGen->GetFVector2DfromFVector(TargetNode->GetActorLocation());
			
			if (TargetNode->isTeleport)
			{
				this->CurrentMovementSpeed = TunnelSpeed;
				//IN TUNNEL SLOW DOWN
				//const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), 0.6 * CurrentMovementSpeed);
				//const FVector Location(Pos.X, Pos.Y, GetActorLocation().Z);
				//SetActorLocation(Location);
			}
			else {
				this->CurrentMovementSpeed = StandardSpeed * 0.75;
			}
		}
	}
} 


void AGhostPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

APacManPawn* AGhostPawn::GetPlayer() const
{	
		return Player;	
}

void AGhostPawn::SetSpeed(float Speed)
{
	CurrentMovementSpeed = Speed;
}

APacManNode* AGhostPawn::GetPlayerRelativeTarget()
{
	 return Player->GetLastNode();;
}

void AGhostPawn::SetGhostTarget()
{   

	const APacManNode* Target = GetPlayerRelativeTarget();
	
	if (!Target)
	{
		Target = GetPlayer()->GetLastNode();
	}

	APacManNode* PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));


	if (PossibleNode)
	{
		this->SetNextNodeByDir(MazeGen->GetFVectorFromFVector2D(PossibleNode->GetNodePosition() - this->GetLastNodeCoords()), true);
	}
}

void AGhostPawn::GhostFrightenedState()
{
	//In frightened state Ghost go at 50% speed
	FrightenedSpeed();

	//Make Ghosts Flash
	MeshHandler();

	//Invert direction to singal frightened state entered
	InvertDirection();


}

void AGhostPawn::FrightenedSpeed()
{
	this->CurrentMovementSpeed = (StandardSpeed / 100) * 50;
}

void AGhostPawn::ChaseScatterSpeed()
{
	this->CurrentMovementSpeed = (StandardSpeed / 100) * 75;
}

void AGhostPawn::InvertDirection()
{
	LastValidInputDirection = LastValidInputDirection * (-1);
}

void AGhostPawn::MeshHandler()
{
	StaticMeshBlue->SetVisibility(true);

	GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::MeshWhite, BlueTime, false);
}

void AGhostPawn::MeshWhite()
{
	StaticMeshWhite->SetVisibility(true);

	GetWorld()->GetTimerManager().SetTimer(WhiteMeshTimer, this, &AGhostPawn::MeshBlue, WhiteTime, false);
}

void AGhostPawn::MeshBlue()
{
	//Increment FlashCounter 
	FlashCounter += 1;
	//Set mesh blue
	StaticMeshWhite->SetVisibility(false);

	if (FlashCounter == 1 || FlashCounter ==2 || FlashCounter == 3)
	{
		GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::MeshWhite, BlueTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::ResetMesh, BlueTime, false);
	}
}

void AGhostPawn::ResetMesh()
{
	//Disable Mesh
	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	//Reset counter
	FlashCounter = 0;
}

void AGhostPawn::SetFrightenedStateTarget()
{
	float RandomNumber = Stream.FRandRange(0, 100);

	if (RandomNumber <= 25)
	{
		 DirCasualVector = FVector(1, 0, 0);
	}
	else if (RandomNumber> 25 && RandomNumber <=50)
	{
		DirCasualVector = FVector(0, 1, 0);
	}
	else if ( RandomNumber > 50 && RandomNumber <= 75)
	{
		 DirCasualVector = FVector(-1, 0, 0);
	}
	else if (RandomNumber > 75 && RandomNumber <= 100)
	{
		 DirCasualVector = FVector(0, -1, 0);
	}

	 RandomNodeCord = CurrentGridCoords + MazeGen->GetFVector2DfromFVector(DirCasualVector);
	 RandomNode = *(MazeGen->TileMap.Find(RandomNodeCord));

	 if (MazeGen->IsNodeValidForWalk(RandomNode))
	 {
		 TargetNode = RandomNode;
	 }
}



void AGhostPawn::RespawnGhost(FVector2D RespawnNode)
{
	StaticMesh->SetVisibility(false);
	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	this->CurrentMovementSpeed = 1200;

	TargetNode = *(MazeGen->TileMap.Find(RespawnNode));

}

void AGhostPawn::ExitRespawnState()
{
	StaticMesh->SetVisibility(true);
	// StopMovement();
	this->ChaseScatterSpeed();
}

void AGhostPawn::StopMovement()
{
/*
	CanMove = false;

	//Ghost haso to go out the base
	this->IsSpawnState = true;

	GetWorld()->GetTimerManager().SetTimer(GhostRestartTimer, this, &AGhostPawn::MoveAgain, 1.0f, false);

*/
}

void AGhostPawn::MoveAgain()
{
	/* 
	CanMove = true;

	APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
	APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));

	EnterNode1->EIsWalkable = Walkable;
	EnterNode2->EIsWalkable = Walkable;

	GetWorld()->GetTimerManager().SetTimer(GhostRestartTimer, this, &AGhostPawn::ExitSpawnStateGhost, 2.0f, false);
	*/
}


void AGhostPawn::ExitSpawnStateGhost()
{
	
	//Make house nodes enter not walkable (19,13) (19,14)

	APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
	APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));
	
	EnterNode1->EIsWalkable = NotWalkable;
	EnterNode2->EIsWalkable = NotWalkable;

	this->IsSpawnState = false;
	
}

void AGhostPawn::GhostRestart(FVector2D SpawnLocation)
{
	FVector SpawnCoord = MazeGen->GetFVectorFromFVector2D(SpawnLocation);
	SpawnCoord.X = SpawnCoord.X * 100 + 50;
	SpawnCoord.Y = SpawnCoord.Y * 100 + 50;
	SpawnCoord.Z =  1.0;
	this->TeleportPawn(SpawnCoord, SpawnLocation);
	this->IsSpawnState = true;
	
	
	APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
	APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));

	EnterNode1->EIsWalkable = NotWalkable;
	EnterNode2->EIsWalkable = NotWalkable;

	GetWorld()->GetTimerManager().SetTimer(GhostRestartTimer, this, &AGhostPawn::ExitSpawnStateGhost, 1.0f, false);	
}


