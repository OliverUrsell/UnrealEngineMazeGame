// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeNode.h"
#include <vector>
#include "GameFramework/Actor.h"
#include "Maze.generated.h"

UCLASS()
class MAZEGAMETRY2_API AMaze : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMaze();
	virtual ~AMaze() override;
	
	MazeNode* Start;
	MazeNode* End;

	MazeNode* GetNodeAtPosition(int X, int Y) const;
	
	UPROPERTY(EditAnywhere)
	int Width = 10;
	
	UPROPERTY(EditAnywhere)
	int Height = 10;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MazeGridBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	std::vector<std::vector<MazeNode*>> Nodes;
	void InitialiseNodes();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	struct FMazeCoordinates {
		// 0,0 Means bottom left
		int X;
		int Y;
	};
	
};
