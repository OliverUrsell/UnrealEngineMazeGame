// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMazeNode.h"
#include <vector>

#include "FMazeGenerator.h"
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
	
	FMazeNode* Start;
	FMazeNode* End;
	std::vector<std::vector<FMazeNode*>> Nodes;

	FMazeNode* GetNodeAtPosition(FMazeCoordinates Coordinates) const;
	
	UPROPERTY(EditAnywhere)
	int Width = 3;
	
	UPROPERTY(EditAnywhere)
	int Depth = 5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ClosedWallsBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CornerExitBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> FourExitBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> OneExitBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> StraightExitBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ThreeExitBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void InitialiseNodes();
	void ConfigureMaze(FMazeGenerator* Generator);
	void SpawnMazeGridBPs() const;
	const int PrefabBlueprintWidth = 1000;
	const int PrefabBlueprintDepth = 1000;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<AActor> GetActorForExits(FNodeExits Exits) const;
	FVector MazeCoordinatesToWorldLocation(FMazeCoordinates Coordinates) const;
	static FRotator GetRotationForExits(FNodeExits Exits);
	
};
