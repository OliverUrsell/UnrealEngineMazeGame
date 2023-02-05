// Fill out your copyright notice in the Description page of Project Settings.

#include "FMazeNode.h"
#include "Maze.h"
#include <cassert>

FMazeNode::FMazeNode(const AMaze* Maze, FMazeCoordinates Coordinates, FNodeExits Exits)
{
	this->Maze = Maze;
	this->Coordinates = Coordinates;
	this->Exits = Exits;
}

TSharedRef<FJsonObject> FMazeNode::ToJSON() const
{
	const TSharedRef<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject);
	
	JsonRootObject->SetNumberField("x", this->Coordinates.X);
	JsonRootObject->SetNumberField("y", this->Coordinates.Y);
	JsonRootObject->SetBoolField("north", this->Exits.bNorth);
	JsonRootObject->SetBoolField("east", this->Exits.bEast);
	JsonRootObject->SetBoolField("south", this->Exits.bSouth);
	JsonRootObject->SetBoolField("west", this->Exits.bWest);

	return JsonRootObject;
}

EMaze_Direction FMazeNode::GetRandomClosedWall() const
{
	assert((void("At least one of the exits must be true"), this->Exits.bNorth||this->Exits.bEast||this->Exits.bSouth||this->Exits.bWest));

	/* initialize random seed: */
	std::srand (time(nullptr));
	
	// node.Coordinates.X += 1;
	while (true)
	{
		const int Random_Number = std::rand() % 4;
		switch(Random_Number)
		{
		case 0:
			if(Exits.bNorth) return North;
			break;
		case 1:
			if(Exits.bEast) return East;
			break;
		case 2:
			if(Exits.bSouth) return South;
			break;
		case 3:
			if(Exits.bWest) return West;
			break;
			
		default:
			// Can't throw an exception here because Unreal Engine does not
			// support building them to Android applications
			GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
				TEXT("Random number generated < 0  or > 3 for random edge generation")));
			return North;
		}
	}
}

EMaze_Direction FMazeNode::GetRandomOpenWall() const
{
	assert((void("At least one of the exits must be false"), !this->Exits.bNorth||!this->Exits.bEast||!this->Exits.bSouth||!this->Exits.bWest));

	/* initialize random seed: */
	std::srand (time(nullptr));
	
	// node.Coordinates.X += 1;
	while (true)
	{
		const int Random_Number = std::rand() % 4;
		switch(Random_Number)
		{
		case 0:
			if(!Exits.bNorth) return North;
			break;
		case 1:
			if(!Exits.bEast) return East;
			break;
		case 2:
			if(!Exits.bSouth) return South;
			break;
		case 3:
			if(!Exits.bWest) return West;
			break;
			
		default:
			// Can't throw an exception here because Unreal Engine does not
			// support building them to Android applications
			GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
				TEXT("Random number generated < 0  or > 3 for random edge generation")));
			return North;
		}
	}
}
