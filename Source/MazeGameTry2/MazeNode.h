// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Maze.h"

/**
 * Class representing one cell / location in the node
 */
class MAZEGAMETRY2_API MazeNode
{
public:

	struct FNodeExits
	{
		bool bNorth=false, bEast=false, bSouth=false, bWest=false;
	};
	
	FNodeExits Exits;
	
	MazeNode(const AMaze* Maze, const AMaze::FMazeCoordinates);
	MazeNode(const AMaze* Maze, const AMaze::FMazeCoordinates, FNodeExits Exits);
	
	AMaze::FMazeCoordinates Coordinates;
	const AMaze* Maze;
};
