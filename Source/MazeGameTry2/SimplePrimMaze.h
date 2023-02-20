// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMazeGenerator.h"

class FMazeNode;
/**
 * 
 */
class MAZEGAMETRY2_API SimplePrimMaze: public FMazeGenerator
{
public:
	struct FMazeNode_Wall
	{
		FMazeNode* Node1;
		FMazeNode* Node2;
	};
	static std::vector<FMazeNode_Wall> GetNodeWalls(const AMaze* Maze, FMazeNode* Node);
	virtual void GenerateMaze(AMaze* Maze) override;
	virtual FMazeNode* GetStartPosition(AMaze* Maze) override;
	virtual FMazeNode* GetEndPosition(AMaze* Maze, FMazeNode* Start) override;
	virtual FMazeNode* GetMonsterPosition(AMaze* Maze, FMazeNode* Start) override;
};
