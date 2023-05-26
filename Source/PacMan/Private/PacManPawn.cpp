// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManPawn.h"
#include "PacManGameMode.h"
#include "Kismet/GameplayStatics.h"

APacManPawn::APacManPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	LastInputDirection = FVector(0,0,0);
	LastValidInputDirection = FVector(0,0,0);

	CurrentGridCoords = FVector2D(8,14);

	NormalMovementSpeed = (StandardSpeed/100)*80;

	//Fruit should spawn near the center of the Maze 
	minX = 6;
	maxX = 21;

	minY = 11;
	maxY = 26;

	Spawnable = false;
	
}
void APacManPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = MazeGen->TileMap[StartNode];

	LifeCounter = 3.0;
	PointCounter = 0.0;
	PointCounterUI = 0.0;

}

void APacManPawn::HandleMovement()
{
	Super::HandleMovement();
	// si muove autonomamente fino a che il next node è walkable
	if (!TargetNode && !NextNode)
	{
		if (MazeGen->IsNodeValidForWalk(MazeGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			SetLastValidDirection(LastInputDirection);
		}
		SetNodeGeneric(LastValidInputDirection);
	}
}


void APacManPawn::SetTargetNode(APacManNode* Node)
{
	Super::SetTargetNode(Node);
}

void APacManPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Point = Cast<APointActor>(OtherActor);
	const auto PowerPoint = Cast<APowerPoint>(OtherActor);
	const auto FruitCast = Cast<AFruitActor>(OtherActor);

	if (Point)
	{
		//Hide actor and disable his tick  
		Point->HideInGame = true;
		Point->DisableActor();

		//Increment point when PacMan collide with PointActor
	    PointCounter += 1;
		PointCounterUI += 10;
	}
	
	if (PowerPoint)
	{
		PowerPoint->HideInGame = true;
		PowerPoint->DisableActor();
		PowerPoint->IsEaten = true;

		PointCounterUI += 150;

		//When a power point is eaten enter Frightened mode 
		GameMode->EnterFrightenedMode();
	}
	
	if (FruitCast)
	{
		Fruit->HideInGame = true;
		Fruit->DisableActor();

		PointCounterUI += 100;
	}
}

void APacManPawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = MazeGen->GetNextNode(CurrentGridCoords, Dir);
	if (MazeGen->IsNodeValidForWalkPacMan(Node))
	{
		SetTargetNode(Node);
	}
}

void APacManPawn::SetNextNodeByDir(FVector InputDir, bool ForceLast)
{

	const FVector2D Coords = TargetNode ? TargetNode->GetNodePosition() : LastNode->GetNodePosition();
	const auto Node = GameMode->GMaze->GetNextNode(Coords, InputDir);
	if (GameMode->GMaze->IsNodeValidForWalkPacMan(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

void APacManPawn::Eat(AGhostPawn* Ghost)
{ 
	Ghost->IsEaten = true;
}


void APacManPawn::RespawnPacMan()
{
	//Teleport PacMan to spawn point

	const FVector Spawn = FVector(850.0f, 1450.0f, GetActorLocation().Z);

	CurrentGridCoords = FVector2D(8,14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(8, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(8, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(Spawn);
 	
}

void APacManPawn::SpawnFruit()
{

	while (!Spawnable)
	{

	 int32 Randx = int32(FMath::FRandRange(minX, maxX));
	 int32 Randy = int32(FMath::FRandRange(minY, maxY));

	 FVector2D FruitCoord = FVector2D(Randx, Randy);
	
	 APacManNode* SpawnFruitNode = *(MazeGen->TileMap.Find(FruitCoord));

	 FVector Position = MazeGen->GetRelativeLocationByXYPosition(Randx,Randy);

	 Spawnable = MazeGen->IsNodeValidForWalkPacMan(SpawnFruitNode);

	 Position.X += 50;
	 Position.Y += 50;

	   if (Spawnable)
	    {
		  Fruit = GetWorld()->SpawnActor<AFruitActor>(FruitActor, Position, FRotator::ZeroRotator);
	    }
	}
	

}

void APacManPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PointCounter == 70 || PointCounter == 170)
	{
		SpawnFruit();
	}
}

void APacManPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection,true);
}

void APacManPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection,true);
}

void APacManPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Actions here
	InputComponent->BindAxis(TEXT("MoveY"), this, &APacManPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveX"), this, &APacManPawn::SetHorizontalInput);
}
