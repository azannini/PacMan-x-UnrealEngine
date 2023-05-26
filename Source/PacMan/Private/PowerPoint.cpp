// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerPoint.h"

void APowerPoint::FlashPoint()
{
	SetActorHiddenInGame(true);

	if (!IsEaten)
		{

	        GetWorld()->GetTimerManager().SetTimer(PointFlash, this, &APowerPoint::ExitFlashPoint, 0.5f, false);
}
		}

void APowerPoint::ExitFlashPoint()
{
	SetActorHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(PointFlash, this, &APowerPoint::FlashPoint, 0.5f, false);
}

void APowerPoint::BeginPlay()
{
	Super::BeginPlay();
	
	bool isEaten = false;

	FlashPoint();

}
