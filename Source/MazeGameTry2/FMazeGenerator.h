// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FMazeNode;
class AMaze;
/**
 * 
 */
class MAZEGAMETRY2_API FMazeGenerator
{
public:
	virtual ~FMazeGenerator(){}
	virtual void GenerateMaze(AMaze* Maze) = 0;
	virtual FMazeNode* GetStartPosition(AMaze* Maze);
	virtual FMazeNode* GetEndPosition(AMaze* Maze, FMazeNode* Start);
	virtual FMazeNode* GetMonsterPosition(AMaze* Maze, FMazeNode* Start);
};
