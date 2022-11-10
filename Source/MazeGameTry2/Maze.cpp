// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"

#include <string>
#include <vector>
#include "FMazeNode.h"
#include "FNodeExits.h"
#include "FMazeGenerator.h"
#include "SimplePrimMaze.h"

// Sets default values
AMaze::AMaze()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AMaze::~AMaze()
{
	for (std::vector<FMazeNode*> Row : Nodes)
	{
		for (const FMazeNode* MazeNode : Row)
		{
			delete(MazeNode);
		}

		Row.clear();
	}
	Nodes.clear();
}

FMazeNode* AMaze::GetNodeAtPosition(FMazeCoordinates Coordinates) const
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
	const bool N = Exits.bNorth;
	const bool E = Exits.bEast;
	const bool S = Exits.bSouth;
	const bool W = Exits.bWest;
	
	const int NumWalls =
		static_cast<int>(N) +
		static_cast<int>(E) +
		static_cast<int>(S) +
		static_cast<int>(W);

	switch (NumWalls)
	{
	case 0:
		return FourExitBP;
	case 1:
		return ThreeExitBP;
	case 2:
		if((N & S) || (E & W)){
			return StraightExitBP;
		}
		return CornerExitBP;
	case 3:
		return OneExitBP;
	case 4:
		return ClosedWallsBP;
	default:
		throw std::runtime_error("There shouldn't be less than 0 or more than 4 walls");
	}
}

FRotator AMaze::GetRotationForExits(FNodeExits Exits)
{
	const bool N = Exits.bNorth;
	const bool E = Exits.bEast;
	const bool S = Exits.bSouth;
	const bool W = Exits.bWest;

	if (
		(!N & !E & !S & !W) || // No Edges
		(N & E & S & W) || // All Edges
		(N & E & !S & W) || // South Exit Only
		(!N & E & !S & W) || // North and South Exit
		(N & E & !S & !W) || // South and West Exit
		(!N & !E & !S & W) // West Wall Only
		)
	{
		return FRotator(0.0,0.0,0.0);
	}

	if(
		(N & E & S & !W) || // West Exit Only
		(N & !E & S & !W) || // East and West Exit
		(!N & E & S & !W) || // North and West Exit
		(N & !E & !S & !W) // North Wall Only
		)
	{
		
		return FRotator(0.0,90.0,0.0);
	}

	if(
		(!N & E & S & W) || // North Exit Only
		(!N & !E & S & W) || // North and East Exit
		(!N & E & !S & !W) // East Wall Only
		)
	{
		return FRotator(0.0,180.0,0.0);
	}

	if(
		(N & !E & S & W) || // East Exit Only
		(N & !E & !S & W) || // East and South Exit
		(!N & !E & S & !W) // South Wall Only
		)
	{
		return FRotator(0.0,270.0,0.0);
	}

	char Error [80];
	sprintf(Error, "No rotation was returned for a set of exit booleans N, S, E, W: %d %d %d %d", N, S, E, W);
	throw std::runtime_error(Error);
}

void AMaze::InitialiseNodes()
{
	// Initialise our nodes 2D vector
	for(float i = 0.0f; i < this->Width; i++)
	{
		std::vector<FMazeNode*> Row;
		for(float j = 0.0f; j < this->Depth; j++)
		{
			Row.push_back(new FMazeNode(this, FMazeCoordinates{j, i}, FNodeExits{}));
		}
		Nodes.push_back(Row);
	}
}

void AMaze::ConfigureMaze(FMazeGenerator* Generator)
{
	Generator->GenerateMaze(this);
}

void AMaze::SpawnMazeGridBPs() const
{
	for (std::vector<FMazeNode*> Row : Nodes)
	{
		for(const FMazeNode* Node : Row)
		{
			GetWorld()->SpawnActor<AActor>(
				this->GetActorForExits(Node->Exits),
				this->MazeCoordinatesToWorldLocation(Node->Coordinates),
				this->GetRotationForExits(Node->Exits)
			);
		}
	}
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();

	this->InitialiseNodes();

	SimplePrimMaze g = SimplePrimMaze();
	this->ConfigureMaze(&g);
	
	this->SpawnMazeGridBPs();
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
