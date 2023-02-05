// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMazeDirection.h"
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

	EMaze_Direction GetRandomClosedWall() const;
	EMaze_Direction GetRandomOpenWall() const;
	
	FMazeCoordinates Coordinates;
	const AMaze* Maze;

	TSharedRef<FJsonObject> ToJSON() const;
};
