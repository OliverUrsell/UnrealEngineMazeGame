// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplePrimMaze.h"
#include "Maze.h"


SimplePrimMaze::SimplePrimMaze()
{
}

void SimplePrimMaze::GenerateMaze(AMaze* Maze)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("Hello World"));
	Maze->End;
}
