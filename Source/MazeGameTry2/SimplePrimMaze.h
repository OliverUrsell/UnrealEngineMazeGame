// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMazeGenerator.h"

/**
 * 
 */
class MAZEGAMETRY2_API SimplePrimMaze: public FMazeGenerator
{
public:
	SimplePrimMaze();
	virtual void GenerateMaze(AMaze* Maze) override;
};
