// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Class representing one cell / location in the node
 */
class MAZEGAMETRY2_API MazeNode
{
public:
	MazeNode(int X, int Y);
	
	bool bNorth = false;
	bool bSouth = false;
	bool bEast = false;
	bool bWest = false;
	int X, Y;  // 0,0 is bottom left  
};
