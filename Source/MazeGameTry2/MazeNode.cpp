// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeNode.h"

#include "Maze.h"

MazeNode::MazeNode(const AMaze* Maze, const AMaze::FMazeCoordinates Coordinates)
{
	// Use all false exits to construct a default node
	MazeNode(Maze, Coordinates, FNodeExits{});
}

MazeNode::MazeNode(const AMaze* Maze, const AMaze::FMazeCoordinates Coordinates, FNodeExits Exits)
{
	this->Exits = Exits;
	this->Coordinates = Coordinates;
	this->Maze = Maze;
}
