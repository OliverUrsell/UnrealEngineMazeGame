// Fill out your copyright notice in the Description page of Project Settings.

#include "FMazeNode.h"
#include "Maze.h"

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
