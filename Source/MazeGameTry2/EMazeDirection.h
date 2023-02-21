#pragma once

UENUM()
//TODO: Make this an enum class "enum class EMaze_Direction"
enum EMaze_Direction
{
	North,East,South,West
};

inline FRotator ToRotation(EMaze_Direction Direction)
{
	switch (Direction)
	{
	case North:
		return FRotator(0, -90, 0);
	case East:
		return FRotator{0, 0, 0};
	case South:
		return FRotator{0, 90, 0};
	case West:
		return FRotator{0, 180, 0};
	default:
		UE_LOG(LogTemp, Error, TEXT("Got Unexpected direction in toRotation"));
		return FRotator(0, 0, 0);
	}
}