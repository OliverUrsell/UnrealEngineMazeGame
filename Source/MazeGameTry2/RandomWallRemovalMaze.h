
#pragma once

#include "CoreMinimal.h"
#include "FMazeGenerator.h"

class MAZEGAMETRY2_API FRandomWallRemovalMaze final : public FMazeGenerator
{
	FMazeGenerator* SubGenerator;
	int WallsToRemove;
	
public:
	explicit FRandomWallRemovalMaze(FMazeGenerator* SubGenerator, int WallsToRemove);
	virtual void GenerateMaze(AMaze* Maze) override;
	virtual FMazeNode* GetStartPosition(AMaze* Maze) override;
	virtual FMazeNode* GetEndPosition(AMaze* Maze, FMazeNode* Start) override;
	virtual FMazeNode* GetMonsterPosition(AMaze* Maze, FMazeNode* Start) override;
};
