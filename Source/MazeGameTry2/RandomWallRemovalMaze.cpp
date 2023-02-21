
#include "RandomWallRemovalMaze.h"

#include "Maze.h"

FRandomWallRemovalMaze::FRandomWallRemovalMaze(FMazeGenerator* SubGenerator, int WallsToRemove)
{
	this->SubGenerator = SubGenerator;
	this->WallsToRemove = WallsToRemove;
}

void FRandomWallRemovalMaze::GenerateMaze(AMaze* Maze)
{
	this->SubGenerator->GenerateMaze(Maze);

	/* initialize random seed: */
	std::srand (time(nullptr));

	for(int i = 0; i < this->WallsToRemove; i++)
	{
		while(true)
		{
			int Random_X = std::rand() % Maze->Depth;
			int Random_Y = std::rand() % Maze->Width;
			
			FMazeNode* CurrentNode = Maze->GetNodeAtPosition(FMazeCoordinates{static_cast<float>(Random_X), static_cast<float>(Random_Y)});
			while(!CurrentNode->WallExists())
			{
				// Keep looking for nodes until we get one with a wall
				Random_X = std::rand() % Maze->Depth;
				Random_Y = std::rand() % Maze->Width;
				CurrentNode = Maze->GetNodeAtPosition(FMazeCoordinates{static_cast<float>(Random_X), static_cast<float>(Random_Y)});
			}

			// Now we have a node with a wall
			EMaze_Direction CurrentDirection = CurrentNode->GetRandomClosedWall();
			FMazeNode* CurrentNeighbour = CurrentNode->GetNeighbour(CurrentDirection);
			if(CurrentNeighbour == nullptr)
			{
				// We have a wall that has a valid node on the other side
				// We did not get a wall that doesn't go out onto the edge
				continue;
			}

			// Remove the wall from the node in that direction
			const int XDiff = CurrentNode->Coordinates.X - CurrentNeighbour->Coordinates.X;
			const int YDiff = CurrentNode->Coordinates.Y - CurrentNeighbour->Coordinates.Y;
			switch(XDiff)
			{
			case -1:
				CurrentNode->Exits.bNorth = false;
				CurrentNeighbour->Exits.bSouth = false;
				break;
			case 1:
				CurrentNode->Exits.bSouth = false;
				CurrentNeighbour->Exits.bNorth = false;
				break;
			case 0:
				switch(YDiff)
				{
					case -1:
						CurrentNode->Exits.bEast = false;
						CurrentNeighbour->Exits.bWest = false;
						break;
					case 1:
						CurrentNode->Exits.bWest = false;
						CurrentNeighbour->Exits.bEast = false;
						break;
					default:
						// Cannot raise an exception here as compiling for VR disables exceptions
						GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::White, TEXT("Wall was defined with a gap of more than 1"));
				}
				break;
			default:
				// Cannot raise an exception here as compiling for VR disables exceptions
				GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::White, TEXT("Wall was defined with a gap of more than 1"));
				break;
			}
			
			break;
		}
	}
}

FMazeNode* FRandomWallRemovalMaze::GetStartPosition(AMaze* Maze){return this->SubGenerator->GetStartPosition(Maze);}

FMazeNode* FRandomWallRemovalMaze::GetEndPosition(AMaze* Maze, FMazeNode* Start){return this->SubGenerator->GetEndPosition(Maze, Start);}

FMazeNode* FRandomWallRemovalMaze::GetMonsterPosition(AMaze* Maze, FMazeNode* Start){return this->SubGenerator->GetMonsterPosition(Maze, Start);};
