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

	// Set the start to the bottom left
	Maze->Start = Start;

	// Set the end to the top right
	Maze->End = Maze->GetNodeAtPosition(FMazeCoordinates{static_cast<float>(Maze->Depth-1), static_cast<float>(Maze->Width-1)});
}
