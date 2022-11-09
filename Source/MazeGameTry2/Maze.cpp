// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"

#include <string>
#include <vector>
#include "MazeNode.h"
#include "FNodeExits.h"

// Sets default values
AMaze::AMaze()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AMaze::~AMaze()
{
	for (std::vector<MazeNode*> Row : Nodes)
	{
		for (const MazeNode* MazeNode : Row)
		{
			delete(MazeNode);
		}

		Row.clear();
	}
	Nodes.clear();
}

MazeNode* AMaze::GetNodeAtPosition(FMazeCoordinates Coordinates) const
{
	return this->Nodes[Coordinates.Y][Coordinates.X];
}

FVector AMaze::MazeCoordinatesToWorldLocation(FMazeCoordinates Coordinates) const
{
	const FVector MazeRootLocation = this->GetActorLocation();
	
	const FVector PositionOffset = FVector{
		this->PrefabBlueprintWidth*Coordinates.X,
		this->PrefabBlueprintDepth*Coordinates.Y,
		0.0
	};
	
	return MazeRootLocation + PositionOffset;
}

TSubclassOf<AActor> AMaze::GetActorForExits(FNodeExits Exits) const
{
	return MazeGridBP;
}

void AMaze::InitialiseNodes()
{
	// Initialise our nodes 2D vector
	for(float i = 0.0f; i < this->Width; i++)
	{
		std::vector<MazeNode*> Row;
		for(float j = 0.0f; j < this->Depth; j++)
		{
			Row.push_back(new MazeNode(this, FMazeCoordinates{j, i}, FNodeExits{}));
		}
		Nodes.push_back(Row);
	}
}

void AMaze::SpawnMazeGridBPs() const
{
	for (std::vector<MazeNode*> Row : Nodes)
	{
		for(MazeNode* Node : Row)
		{
			GetWorld()->SpawnActor<AActor>(
				this->GetActorForExits(Node->Exits),
				this->MazeCoordinatesToWorldLocation(Node->Coordinates),
				this->GetActorRotation()
			);
		}
	}
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();

	this->InitialiseNodes();
	this->SpawnMazeGridBPs();
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
