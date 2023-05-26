// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManPlayerController.h"

APacManPlayerController::APacManPlayerController()
{
	bShowMouseCursor = true;

}

void APacManPlayerController::BeginPlay()
{
}

void APacManPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//Bind grid movement Actions here
	InputComponent->BindAxis(TEXT("MoveY"), this, &APacManPlayerController::SetVertical);
	InputComponent->BindAxis(TEXT("MoveX"), this, &APacManPlayerController::SetHorizontal);

}

void  APacManPlayerController::SetHorizontal(float Amount)
{
	const auto P = Cast<AMazePawn>(GetPawn());
	if (P)
	{
		P->SetHorizontalInput(Amount);
	}

}

void  APacManPlayerController::SetVertical(float Amount)
{
	const auto P = Cast<AMazePawn>(GetPawn());
	if (P)
	{
		P->SetVerticalInput(Amount);
	}
}
