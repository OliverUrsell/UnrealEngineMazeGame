// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Maze.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMonster::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector CurrentLocation = this->GetActorLocation();
	const FVector GoalLocation = GoalNode->Maze->MazeCoordinatesToWorldLocation(GoalNode->Coordinates);
	
	FVector GoalVector = GoalLocation - CurrentLocation;

	GoalVector.Z = 0;

	FVector GoalMovementDirection = GoalVector;
	GoalMovementDirection.Normalize();
	FVector NewLocation = CurrentLocation + (GoalMovementDirection*this->Speed*DeltaTime);
	NewLocation.Z = this->GoalNode->Maze->MonsterPositionHeight;

	// Compare squared values to avoid a square root
	if(GoalVector.SizeSquared() <= this->ThresholdMagnitude*this->ThresholdMagnitude) UpdateGoalLocation();

	//TODO: don't update the location if there isn't much movement to be done, avoids jitter
	this->SetActorLocation(NewLocation);

	FRotator CurrentRotation = this->GetActorRotation();
	FRotator GoalRotation = ToRotation(CurrentDirection);

	FRotator RotationChange = (GoalRotation - CurrentRotation)*this->RotationSpeed*DeltaTime;
	if(!RotationChange.IsNearlyZero()){this->SetActorRotation(CurrentRotation + RotationChange);}
	
}

void AMonster::UpdateGoalLocation() const
{
	// Assume we are nearby the current goal location, we want to find out if we need to change the location
	if(!this->GoalNode->GetWallStatus(this->CurrentDirection))
	{
		this->GoalNode = this->GoalNode->GetNeighbour(this->CurrentDirection);
	}
}

void AMonster::DirectionChanged(const EMaze_Direction Direction)
{
	CurrentDirection = Direction;
}


