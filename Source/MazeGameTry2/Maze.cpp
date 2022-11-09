// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"

#include <string>
#include <vector>
#include "MazeNode.h"

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

MazeNode* AMaze::GetNodeAtPosition(const int X, const int Y) const
{
	return this->Nodes[Y][X];
}

void AMaze::InitialiseNodes()
{
	// Initialise our nodes 2D vector
	for(int i = 0; i <= this->Height; i++)
	{
		std::vector<MazeNode*> Row;
		for(int j = 0; j <= this->Width; j++)
		{
			Row.push_back(new MazeNode(j, i));
		}
		Nodes.push_back(Row);
	}
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();

	this->InitialiseNodes();
	
	GetWorld()->SpawnActor<AActor>(MazeGridBP, GetActorLocation(), GetActorRotation());
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
