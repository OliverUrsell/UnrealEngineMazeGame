// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeNode.h"
#include "Maze.h"

MazeNode::MazeNode(const AMaze* Maze, FMazeCoordinates Coordinates, const FNodeExits Exits)
{
	this->Maze = Maze;
	this->Coordinates = Coordinates;
	this->Exits = Exits;
}
