// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplePrimMaze.h"

#include <unordered_set>

#include "Maze.h"

std::vector<SimplePrimMaze::FMazeNode_Wall> SimplePrimMaze::GetNodeWalls(const AMaze* Maze, FMazeNode* Node)
{
	std::vector<FMazeNode_Wall> Walls = {};
	const FMazeCoordinates Coordinates = Node->Coordinates;
	
	if(Coordinates.X > 0)
	{
		FMazeNode* Node2 = Maze->GetNodeAtPosition(FMazeCoordinates{Coordinates.X - 1, Coordinates.Y});
		Walls.push_back(FMazeNode_Wall{Node, Node2});
	}
	
	if(Coordinates.X < Maze->Depth - 1)
	{
		FMazeNode* Node2 = Maze->GetNodeAtPosition(FMazeCoordinates{Coordinates.X + 1, Coordinates.Y});
		Walls.push_back(FMazeNode_Wall{Node, Node2});
	}

	if(Coordinates.Y > 0)
	{
		FMazeNode* Node2 = Maze->GetNodeAtPosition(FMazeCoordinates{Coordinates.X, Coordinates.Y - 1});
		Walls.push_back(FMazeNode_Wall{Node, Node2});
	}

	if(Coordinates.Y < Maze->Width - 1)
	{
		FMazeNode* Node2 = Maze->GetNodeAtPosition(FMazeCoordinates{Coordinates.X, Coordinates.Y + 1});
		Walls.push_back(FMazeNode_Wall{Node, Node2});
	}

	return Walls;
}

void SimplePrimMaze::GenerateMaze(AMaze* Maze)
{
	
	FMazeNode* Start = Maze->GetNodeAtPosition(FMazeCoordinates{0,0});
	
	std::unordered_set<const FMazeNode*> Visited = {};
	Visited.insert(Start);
	
	std::vector<FMazeNode_Wall> Walls = GetNodeWalls(Maze, Start);
	
	while(!Walls.empty())
	{
		// Select a random wall and get it's two nodes
		const int SelectedIndex = std::rand() % Walls.size();
		const FMazeNode_Wall Selected = Walls[SelectedIndex];
		FMazeNode* Node1 = Selected.Node1;
		FMazeNode* Node2 = Selected.Node2;

		bool Node1Visited = Visited.find(Node1) != Visited.end();
		bool Node2Visited = Visited.find(Node2) != Visited.end();

		if(Node1Visited != Node2Visited)
		{
			const int XDiff = Node1->Coordinates.X - Node2->Coordinates.X;
			const int YDiff = Node1->Coordinates.Y - Node2->Coordinates.Y;
			switch(XDiff)
			{
			case -1:
				Node1->Exits.bNorth = false;
				Node2->Exits.bSouth = false;
				break;
			case 1:
				Node1->Exits.bSouth = false;
				Node2->Exits.bNorth = false;
				break;
			case 0:
				switch(YDiff)
				{
				case -1:
					Node1->Exits.bEast = false;
					Node2->Exits.bWest = false;
					break;
				case 1:
					
					Node1->Exits.bWest = false;
					Node2->Exits.bEast = false;
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

			std::vector<FMazeNode_Wall> NewWalls;
			
			if(!Node1Visited)
			{
				NewWalls = GetNodeWalls(Maze, Node1);
				Visited.insert(Node1);
			}
			else
			{
				NewWalls = GetNodeWalls(Maze, Node2);
				Visited.insert(Node2);
			}
			
			for(FMazeNode_Wall Wall : NewWalls)
			{
				Walls.push_back(Wall);
			}
			
		}

		// Remove the Wall at the selected index from Walls
		Walls.erase(Walls.begin() + SelectedIndex);
	}

	// Set the start position
	Maze->Start = GetStartPosition(Maze);

	// Set the end position
	Maze->End = GetEndPosition(Maze, Maze->Start);
	while(!Maze->End->WallExists())
	{
		// Keep going until you find a node that has a wall, otherwise the goal cannot be placed
		Maze->End = GetEndPosition(Maze, Maze->Start);
	}

	Maze->MonsterStart = GetMonsterPosition(Maze, Maze->Start);
}

FMazeNode* SimplePrimMaze::GetStartPosition(AMaze* Maze)
{
	// Get a start position on the edges of the map
	constexpr int Edge_Depth = 2;
	
	/* initialize random seed: */
	std::srand (time(nullptr));

	const int Orientation = std::rand() % 2;
	int Random_X, Random_Y;
	switch (Orientation)
	{
	case 0:
		// Generate a point along the top or bottom
		Random_X = std::rand() % Edge_Depth*2;
		Random_Y = std::rand() % Maze->Width;
		// If Random is in the second half map this to the opposite edge of the maze
		if(Random_X >= Edge_Depth)
		{
			Random_X = Maze->Depth - (Random_X - Edge_Depth) - 1;
		}
		break;
	case 1:
		// Generate a point along the left or right
		Random_X = std::rand() % Maze->Depth;
		Random_Y = std::rand() % Edge_Depth*2;
		// If Random is in the second half map this to the opposite edge of the maze
		if(Random_Y >= Edge_Depth)
		{
			Random_Y = Maze->Width - (Random_Y - Edge_Depth) - 1;
		}
		break;
	default:
		// We got a value outside of the expected values for Orientation
		exit(1);
	}

	UE_LOG(LogTemp, Error, TEXT("Start Position: %d, %d"), Random_X, Random_Y);
	
	return Maze->GetNodeAtPosition(FMazeCoordinates{static_cast<float>(Random_X), static_cast<float>(Random_Y)});
}

FMazeNode* SimplePrimMaze::GetEndPosition(AMaze* Maze, FMazeNode* Start)
{
	// Get a goal position on the edges of the map, on the opposite quadrant from the player
	
	/* initialize random seed: */
	std::srand (time(nullptr));
	constexpr int Edge_Depth = 2;

	const int Orientation = std::rand() % 2;
	int Random_X, Random_Y;
	
	switch (Orientation)
	{
	case 0:
		// Generate a point along the top or bottom
		Random_X = std::rand() % Edge_Depth;
		Random_Y = std::rand() % Maze->Width/2;
		break;
	case 1:
		// Generate a point along the left or right
		Random_X = std::rand() % Maze->Depth/2;
		Random_Y = std::rand() % Edge_Depth;
		break;
	default:
		// We got a value outside of the expected values for Orientation
		exit(1);
		return nullptr;
	}

	int Out_X, Out_Y;

	// If start x position is greater than half the depth, put the Maze position in the first 3 of the edges
	// else put it in the end half

	if(Start->Coordinates.X >= Maze->Depth/2)
	{
		Out_X = Random_X;
	}
	else
	{
		Out_X = Maze->Depth - Random_X - 1;
	}

	// If start y position is greater than half the width, put the maze position in the first 3 of the edges
	// else put it in the end half

	if(Start->Coordinates.Y >= Maze->Width/2)
	{
		Out_Y = Random_Y;
	}
	else
	{
		Out_Y = Maze->Width - Random_Y - 1;
	}

	UE_LOG(LogTemp, Error, TEXT("End Position: %d, %d"), Out_X, Out_Y);
	return Maze->GetNodeAtPosition(FMazeCoordinates{static_cast<float>(Out_X), static_cast<float>(Out_Y)});
}

FMazeNode* SimplePrimMaze::GetMonsterPosition(AMaze* Maze, FMazeNode* Start)
{
	// Get a monster position outside of the quadrant the player is in
	
	/* initialize random seed: */
	std::srand (time(nullptr));

	const int Orientation = std::rand() % 2;
	int Random_X, Random_Y;
	
	switch (Orientation)
	{
	case 0:
		// Avoid the player in the X direction
		Random_Y = std::rand() % Maze->Width;
		if(Start->Coordinates.X >= Maze->Depth/2)
		{
			Random_X = std::rand() % Maze->Depth/2;
		}else
		{
			Random_X = Maze->Depth/2 + std::rand() % Maze->Depth/2;
		}
		break;
	case 1:
		// Avoid the player in the Y direction
		Random_X = std::rand() % Maze->Depth;
		if(Start->Coordinates.Y >= Maze->Width/2)
		{
			Random_Y = std::rand() % Maze->Width/2;
		}else
		{
			Random_Y = Maze->Width/2 + std::rand() % Maze->Width/2;
		}
		break;
	default:
		// We got a value outside of the expected values for Orientation
		exit(1);
		return nullptr;
	}

	UE_LOG(LogTemp, Error, TEXT("Monster Position: %d, %d"), Random_X, Random_Y);
	return Maze->GetNodeAtPosition(FMazeCoordinates{static_cast<float>(Random_X), static_cast<float>(Random_Y)});
}
