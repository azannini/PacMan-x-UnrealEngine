// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenerator.h"

constexpr int MapSizeX = 36;
constexpr int MapSizeY = 29;

//   - Empty (LabyrinthNode)
// # - Wall
// B - Big Point (PowerNode)
// o - Point
// O - Outside Node
// I - GhostBaseNode

static char Map[MapSizeX][MapSizeY] = {


	"OOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"OOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"############################",
	"#oooooooooooooooooooooooooo#",
	"#o##########o##o##########o#",
	"#o#######oooo##oooo#######o#",
	"#oooooo##o########o##oooooo#",
	"###o##o##o########o##o##o###",
	"#Boo##ooooooo  ooooooo##ooB#",
	"#o####o#####o##o#####o####o#",
	"#o####o#####o##o#####o####o#",
	"#oooooooooooo##oooooooooooo#",
	"######o## ######## ##o######",
	"OOOOO#o## ######## ##o#OOOOO",
	"OOOOO#o##          ##o#OOOOO",
	"OOOOO#o## ######## ##o#OOOOO",
	"######o## #OOOOOO# ##o######",
	"TTTTTTo   #OOOOOO#   oTTTTTT",
	"######o## #OOOOOO# ##o######",
	"OOOOO#o## ###II### ##o#OOOOO",
	"OOOOO#o##          ##o#OOOOO",
	"OOOOO#o##### ## #####o#OOOOO",
	"######o##### ## #####o######",
	"#oooooo##oooo##oooo##oooooo#",
	"#o####o##o########o##o####o#",
	"#o####o##o########o##o####o#",
	"#oooooooooooooooooooooooooo#",
	"#o####o#####o##o#####o####o#",
	"#B#OO#o#OOO#o##o#OOO#o#OO#B#",
	"#o####o#####o##o#####o####o#",
	"#oooooooooooo##oooooooooooo#",
	"############################",
	"OOOOOOOOOOOOOOOOOOOOOOOOOOOO",
	"OOOOOOOOOOOOOOOOOOOOOOOOOOOO",
};

// Sets default values
AMazeGenerator::AMazeGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NodeSize = 100.0f;
	SpawnOffset = FVector(NodeSize);

}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
	GenerateMaze();

}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMazeGenerator::GenerateMaze()
{

	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY - 1; y++)
		{
			const char MapTile = Map[x][y];

			FVector OffsetVector(x * SpawnOffset.X, y * SpawnOffset.Y, 0);
			const FVector CurrentSpawnPosition = GetActorLocation() + OffsetVector;
			// Spawn a new tile
			const auto SpawnedNode = SpawnNodeActorById(MapTile, CurrentSpawnPosition);
			// Set Maze coordinates to the the previous tile
			SpawnedNode->NodeGridPosition = (FVector2D(x, y));
			// Set World coordinates to the the previous tile
			SpawnedNode->NodeCoordinatesPosition = CurrentSpawnPosition;
			// Tile added to the array and map
			Node.Add(SpawnedNode);
			TileMap.Add(FVector2D(x, y), SpawnedNode);
		}
	}
}

APacManNode* AMazeGenerator::SpawnNodeActorById(char CharId, FVector Position) const
{
	APacManNode* Node_tmp;
	TSubclassOf<APacManNode> ClassToSpawn = APacManNode::StaticClass();


	switch (CharId)
	{
	case '#': ClassToSpawn = WallNode; break;
	case 'B': ClassToSpawn = PowerNode; break;
	case 'T': ClassToSpawn = TeleportNode; break;
	case 'I': ClassToSpawn = InvisibleWallNode; break;
	case 'o': ClassToSpawn = PointNode; break;
	case 'O': ClassToSpawn = OutsideNode; break;
	default: ClassToSpawn = LabyrinthNode; break;
	}

	//Check to verify if it should spawn also a point actor

	if (ClassToSpawn == PointNode)
	{
		GetWorld()->SpawnActor<APointActor>(Point, Position, FRotator::ZeroRotator);
	}
	if (ClassToSpawn == PowerNode)
	{
		GetWorld()->SpawnActor<APointActor>(PowerPoint, Position, FRotator::ZeroRotator);
	}

	Node_tmp = GetWorld()->SpawnActor<APacManNode>(ClassToSpawn, Position, FRotator::ZeroRotator);


	return Node_tmp;
}


TMap<FVector2D, APacManNode*> AMazeGenerator::GetTileMap()
{
	return TileMap;
}

FVector2D AMazeGenerator::GetPosition(const FHitResult& Hit)
{
	return Cast<APacManNode>(Hit.GetActor())->GetNodePosition();
}

TArray<APacManNode*>& AMazeGenerator::GetTileArray()
{
	return Node;
}

FVector AMazeGenerator::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)
{
	return  NodeSize * FVector(InX, InY, 0);
}

FVector2D AMazeGenerator::GetXYPositionByRelativeLocation(const FVector& Location)
{
	double x = floor(Location[0] / (NodeSize));
	double y = floor(Location[1] / (NodeSize));
	return FVector2D(x, y);

}

bool AMazeGenerator::IsNodeValidForWalk(APacManNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->EIsWalkable == NotWalkable) return false;

	return true;
}

APacManNode* AMazeGenerator::GetNextNode(const FVector2D StartCoords, FVector InputDir)
{

	const float RequestedX = StartCoords.X + InputDir.X;
	const float RequestedY = StartCoords.Y + InputDir.Y;

	//Return a number in a given interval 

	const float ClampedX = FMath::Clamp(RequestedX, 0.f, MapSizeX - 1);
	const float ClampedY = FMath::Clamp(RequestedY, 0.f, MapSizeY - 2);

	APacManNode* PossibleNode = GetTileMap()[FVector2D(ClampedX, ClampedY)];

	if (RequestedX > ClampedX || RequestedX < 0)
	{
		//X overflow
		PossibleNode = nullptr;
	}
	if (RequestedY > ClampedY || RequestedY < 0)
	{
		//Y overflow
		PossibleNode = nullptr;
	}
	return PossibleNode;
}

FVector2D AMazeGenerator::GetFVector2DfromFVector(FVector DDDVector)
{
	return FVector2D(DDDVector.X, DDDVector.Y);;
}

FVector AMazeGenerator::GetFVectorFromFVector2D(FVector2D DDDVector)
{
	return FVector(DDDVector.X, DDDVector.Y, 0);
}



//Functions to handle Ghost movement


TArray<FDirNode> AMazeGenerator::NearPossibleNode(const APacManNode* NodeInp)
{

	TArray<FDirNode> Vec;
	if (NodeInp)
	{
		Vec.Add(FDirNode(GetNextNode(NodeInp->GetNodePosition(), FVector::ForwardVector), FVector::ForwardVector));
		Vec.Add(FDirNode(GetNextNode(NodeInp->GetNodePosition(), FVector::RightVector), FVector::RightVector));
		Vec.Add(FDirNode(GetNextNode(NodeInp->GetNodePosition(), -FVector::ForwardVector), -FVector::ForwardVector));
		Vec.Add(FDirNode(GetNextNode(NodeInp->GetNodePosition(), FVector::LeftVector), FVector::LeftVector));
	}
	return Vec;
}

APacManNode* AMazeGenerator::ShortestNodeToTarget(const FVector2D StartCoords, const FVector2D TargetCoords, FVector IgnoredDir)
{
	const TArray<FDirNode> PossibleNodes = NearPossibleNode(GetNodeByCoords(StartCoords));
	APacManNode* ShortestNode = nullptr;
	float Distance = 100000000000000.0;

	for (FDirNode FDirNode : PossibleNodes)
	{
		if (FDirNode.Node == nullptr || FDirNode.Dir == IgnoredDir || FDirNode.Node->EIsWalkable == NotWalkable)
			continue;

		const float TempDist = FVector2D::Distance(FDirNode.Node->GetNodePosition(), TargetCoords);
		if (TempDist < Distance)
		{
			Distance = TempDist;
			ShortestNode = FDirNode.Node;
		}
	}

	return ShortestNode;

}

bool AMazeGenerator::IsNodeReachableAndNextToCurrentPosition(const FVector2D CurrentCoordinates, const FVector2D TargetCoords)
{
	const float DistX = FMath::Abs(CurrentCoordinates.X - TargetCoords.X);
	const float DistY = FMath::Abs(CurrentCoordinates.Y - TargetCoords.Y);
	if (DistX > 1 || DistY > 1) return false;
	APacManNode* const N = GetNodeByCoords(TargetCoords);
	if (N && N->EIsWalkable == NotWalkable) return false;
	return true;
}

APacManNode* AMazeGenerator::GetNodeByCoords(const FVector2D Coords)
{
	if (Coords.X > MapSizeX - 1 || Coords.Y > MapSizeY - 2) return nullptr;
	if (Coords.X < 0 || Coords.Y < 0) return nullptr;
	return GetTileMap()[FVector2D(Coords.X, Coords.Y)];
}

bool AMazeGenerator::IsNodeValidForWalkPacMan(APacManNode* Node)
{
    if (Node == nullptr) return false;
	if (Node->EIsWalkable == NotWalkable) return false;

	return true;
}
