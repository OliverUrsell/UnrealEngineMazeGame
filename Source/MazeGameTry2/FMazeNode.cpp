// Fill out your copyright notice in the Description page of Project Settings.

#include "FMazeNode.h"
#include "Maze.h"
#include <cassert>

FMazeNode::FMazeNode(const AMaze* Maze, const FMazeCoordinates Coordinates, FNodeExits Exits)
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

bool FMazeNode::WallExists() const
{
	return GetWallStatus(North) || GetWallStatus(East) || GetWallStatus(South) || GetWallStatus(West); 
}

bool FMazeNode::GetWallStatus(const EMaze_Direction Direction) const
{
	switch (Direction)
	{
	case North: return this->Exits.bNorth;
	case East: return this->Exits.bEast;
	case South: return this->Exits.bSouth;
	case West: return this->Exits.bWest;
	default:
		// Can't throw an exception here because Unreal Engine does not
		// support building them to Android applications
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
			TEXT("Got an unexpected value for the direction")));;
		return false;
	}
}

FMazeNode* FMazeNode::GetNeighbour(const EMaze_Direction Direction) const
{
	const float X_Position = this->Coordinates.X;
	const float Y_Position = this->Coordinates.Y;
	switch (Direction)
	{
	case North: return this->Maze->GetNodeAtPosition({X_Position + 1, Y_Position});
	case East: return this->Maze->GetNodeAtPosition({X_Position, Y_Position + 1});
	case South: return this->Maze->GetNodeAtPosition({X_Position - 1, Y_Position});
	case West: return this->Maze->GetNodeAtPosition({X_Position, Y_Position - 1});
	default:
		// Can't throw an exception here because Unreal Engine does not
		// support building them to Android applications
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
			TEXT("Got an unexpected value for the direction")));;
		return nullptr;
	}
}
