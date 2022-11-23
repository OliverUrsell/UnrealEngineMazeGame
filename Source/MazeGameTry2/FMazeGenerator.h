// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AMaze;
/**
 * 
 */
class MAZEGAMETRY2_API FMazeGenerator
{
public:
	virtual ~FMazeGenerator(){}
	virtual void GenerateMaze(AMaze* Maze) = 0;
};
