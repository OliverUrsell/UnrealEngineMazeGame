// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMazeCoordinates.h"
#include "FNodeExits.h"

class AMaze;

/**
 * Class representing one cell / location in the node
 */
class MAZEGAMETRY2_API FMazeNode
{
public:
	FNodeExits Exits;
	
	FMazeNode(const AMaze* Maze, FMazeCoordinates Coordinates, FNodeExits Exits);
	
	FMazeCoordinates Coordinates;
	const AMaze* Maze;
};
