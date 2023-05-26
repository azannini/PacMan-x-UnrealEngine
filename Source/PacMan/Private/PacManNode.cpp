// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManNode.h"

// Sets default values
APacManNode::APacManNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isTeleport = false;
}

// Called when the game starts or when spawned
void APacManNode::BeginPlay()
{
	Super::BeginPlay();

}

void APacManNode::SetNodePosition(const double InX, const double InY)
{
	return NodeGridPosition.Set(InX, InY);
}

FVector2D APacManNode::GetNodePosition() const
{
	return NodeGridPosition;
}

FVector APacManNode::GetNodeCoordinates()
{
	return NodeCoordinatesPosition;
}

void APacManNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}