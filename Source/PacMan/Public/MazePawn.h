// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacManNode.h"
#include "MazeGenerator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "MazePawn.generated.h"

UCLASS()
class PACMAN_API AMazePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMazePawn();

	// Template functions, explicitly left empty for child classes to override
	virtual void SetVerticalInput(float AxisValue);
	virtual void SetHorizontalInput(float AxisValue);

	UFUNCTION(BlueprintCallable)
	virtual void SetNextNodeByDir(FVector InputDir, bool ForceLast = false);

	FVector GetLastValidDirection() const;

	UFUNCTION(BlueprintCallable)
		APacManNode* GetLastNode() const;
	UFUNCTION(BlueprintCallable)
		APacManNode* GetTargetNode() const;
	UFUNCTION(BlueprintCallable)
		FVector2D GetLastNodeCoords() const;
	UFUNCTION(BlueprintCallable)
		FVector2D GetTargetNodeCoords() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastValidInputDirection;

	void SetLastValidDirection(FVector Dir);


	UPROPERTY(EditAnywhere)
		float AcceptedDistance = 4.f;

	UPROPERTY(VisibleAnywhere)
		FVector2D CurrentGridCoords;

	UPROPERTY(VisibleAnywhere)
		class APacManGameMode* GameMode;

	UPROPERTY(VisibleAnywhere)
		AMazeGenerator* MazeGen;

	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		APacManNode* NextNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		APacManNode* TargetNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		APacManNode* LastNode;

	virtual void HandleMovement();
	virtual void OnNodeReached();
			void MoveToCurrentTargetNode();
	virtual void SetTargetNode(APacManNode* Node);
	virtual void SetNextNode(APacManNode* Node);
	virtual void SetNodeGeneric(const FVector Dir);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;

	UPROPERTY(VisibleAnywhere)
		bool CanMove;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TeleportPawn(FVector Location,FVector2D SpawnNode);

	UPROPERTY(EditAnywhere, Category = "Movement")
		float CurrentMovementSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float NormalMovementSpeed = 100.0f;

	UPROPERTY()
		float StandardSpeed = 800.0f;
};
