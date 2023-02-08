// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMazeDirection.h"
#include "FMazeNode.h"
#include "GameFramework/Actor.h"
#include "Monster.generated.h"

UCLASS()
class MAZEGAMETRY2_API AMonster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMaze_Direction> CurrentDirection;

public:
	const float ThresholdMagnitude = 25.0f;
	float Speed = 100.0f;
	
	mutable FMazeNode* GoalNode;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateGoalLocation() const;

	// Called whenever the monster's direction changes
	void DirectionChanged(EMaze_Direction Direction);
};
