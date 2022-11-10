// Fill out your copyright notice in the Description page of Project Settings.

#include "FMazeNode.h"
#include "Maze.h"

FMazeNode::FMazeNode(const AMaze* Maze, FMazeCoordinates Coordinates, FNodeExits Exits)
{
	this->Maze = Maze;
	this->Coordinates = Coordinates;
	this->Exits = Exits;
}
