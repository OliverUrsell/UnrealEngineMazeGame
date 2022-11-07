// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeNode.h"


MazeNode::MazeNode(const int X, const int Y)
{
	bNorth = false;
	bSouth = false;
	bEast = false;
	bWest = false;

	this->X = X;
	this->Y = Y;
}
