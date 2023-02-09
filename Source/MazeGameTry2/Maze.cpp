// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"

#include <string>
#include <vector>
#include "FMazeNode.h"
#include "FNodeExits.h"
#include "FMazeGenerator.h"
#include "JsonObjectConverter.h"
#include "Monster.h"
#include "SimplePrimMaze.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Kismet/GameplayStatics.h"
#include "ServerSocketClient.h"

class AMonster;
// Sets default values
AMaze::AMaze()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AMaze::~AMaze()
{
	for (std::vector<FMazeNode*> Row : Nodes)
	{
		for (const FMazeNode* MazeNode : Row)
		{
			delete(MazeNode);
		}

		Row.clear();
	}
	Nodes.clear();
}

FMazeNode* AMaze::GetNodeAtPosition(FMazeCoordinates Coordinates) const
{
	//TODO: Add an assert here to check the coordinates are valid
	return this->Nodes[Coordinates.Y][Coordinates.X];
}

FVector AMaze::MazeCoordinatesToWorldLocation(const FMazeCoordinates Coordinates) const
{
	const FVector MazeRootLocation = this->GetActorLocation();
	
	const FVector PositionOffset = FVector{
		this->PrefabBlueprintWidth*Coordinates.X,
		this->PrefabBlueprintDepth*Coordinates.Y,
		0.0
	};
	
	return MazeRootLocation + PositionOffset;
}

FMazeCoordinates AMaze::WorldLocationToMazeCoordinates(const FVector Location) const
{
	//TODO: Add an assert here to check the coordinates are valid
	const FVector MazeRootLocation = this->GetActorLocation();
	const FVector PositionOffset = Location - MazeRootLocation;

	return FMazeCoordinates{
		PositionOffset.X / this->PrefabBlueprintWidth,
		PositionOffset.Y / this->PrefabBlueprintDepth,
	};
}

TSubclassOf<AActor> AMaze::GetActorForExits(FNodeExits Exits) const
{
	const bool N = Exits.bNorth;
	const bool E = Exits.bEast;
	const bool S = Exits.bSouth;
	const bool W = Exits.bWest;
	
	const int NumWalls =
		static_cast<int>(N) +
		static_cast<int>(E) +
		static_cast<int>(S) +
		static_cast<int>(W);

	switch (NumWalls)
	{
	case 0:
		return FourExitBP;
	case 1:
		return ThreeExitBP;
	case 2:
		if((N & S) || (E & W)){
			return StraightExitBP;
		}
		return CornerExitBP;
	case 3:
		return OneExitBP;
	case 4:
		return ClosedWallsBP;
	default:
		// Can't throw an exception here because Unreal Engine does not
		// support building them to Android applications
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
			TEXT("There shouldn't be less than 0 or more than 4 walls")));
	}

	return ClosedWallsBP;
}

FRotator AMaze::GetRotationForExits(const FNodeExits Exits)
{
	const bool N = Exits.bNorth;
	const bool E = Exits.bEast;
	const bool S = Exits.bSouth;
	const bool W = Exits.bWest;

	if (
		(!N & !E & !S & !W) || // No Edges
		(N & E & S & W) || // All Edges
		(N & E & !S & W) || // South Exit Only
		(!N & E & !S & W) || // North and South Exit
		(N & E & !S & !W) || // South and West Exit
		(!N & E & !S & !W) // East Wall Only
		)
	{
		return FRotator(0.0,0.0,0.0);
	}

	if(
		(N & E & S & !W) || // West Exit Only
		(N & !E & S & !W) || // East and West Exit
		(!N & E & S & !W) || // North and West Exit
		(!N & !E & S & !W) // South Wall Only
		)
	{
		
		return FRotator(0.0,90.0,0.0);
	}

	if(
		(!N & E & S & W) || // North Exit Only
		(!N & !E & S & W) || // North and East Exit
		(!N & !E & !S & W) // West Wall Only
		)
	{
		return FRotator(0.0,180.0,0.0);
	}

	if(
		(N & !E & S & W) || // East Exit Only
		(N & !E & !S & W) || // East and South Exit
		(N & !E & !S & !W) // North Wall Only
		)
	{
		return FRotator(0.0,270.0,0.0);
	}

	// Can't throw an exception here because Unreal Engine does not
	// support building them to Android applications
	char Error [80];
	sprintf(Error, "No rotation was returned for a set of exit booleans N, S, E, W: %d %d %d %d", N, S, E, W);
	GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(Error));

	return FRotator();
}

std::pair<FMazeCoordinates, FRotator> AMaze::GetPositionAndRotationForGoal() const
{
	const EMaze_Direction Direction = End->GetRandomClosedWall();
	switch(Direction)
	{
	case North:
		return {
			FMazeCoordinates{End->Coordinates.X + 0.4f, End->Coordinates.Y},
			FRotator(0.0,0.0,0.0),
		};
	case East:
		return {
			FMazeCoordinates{End->Coordinates.X, End->Coordinates.Y + 0.4f},
			FRotator(0.0,90.0,0.0),
		};
	case South:
		return {
			FMazeCoordinates{End->Coordinates.X - 0.4f, End->Coordinates.Y},
			FRotator(0.0,180.0,0.0),
		};
	case West:
		return {
			FMazeCoordinates{End->Coordinates.X, End->Coordinates.Y - 0.4f},
			FRotator(0.0,270.0,0.0),
		};
	default:
		// Can't throw an exception here because Unreal Engine does not
		// support building them to Android applications
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
			TEXT("Got an invalid value of EMazeDirection")));
		return {
			FMazeCoordinates{End->Coordinates.X, End->Coordinates.Y},
			FRotator(0.0,0.0,0.0),
		};
	}
}

std::pair<FMazeNode*, std::pair<EMaze_Direction, FRotator>>
AMaze::GetMonsterStartingPositionAndRotation() const
{
	/* initialize random seed: */
	std::srand (time(nullptr));
	const int Random_X = std::rand() % this->Depth;
	const int Random_Y = std::rand() % this->Width;
	FMazeNode* Node = this->GetNodeAtPosition(FMazeCoordinates{static_cast<float>(Random_X), static_cast<float>(Random_Y)});
	
	const EMaze_Direction Direction = Node->GetRandomOpenWall();
	switch(Direction)
	{
	case North:
		return {
			Node,
			{Direction, FRotator(0.0,270.0,0.0)},
		};
	case East:
		return {
			Node,
			{Direction, FRotator(0.0,0.0,0.0)},
		};
	case South:
		return {
			Node,
			{Direction, FRotator(0.0,90.0,0.0)},
		};
	case West:
		return {
			Node,
			{Direction, FRotator(0.0,180.0,0.0)},
		};
	default:
		// Can't throw an exception here because Unreal Engine does not
		// support building them to Android applications
		GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
			TEXT("Got an invalid value of EMazeDirection")));
		return {
			Node,
			{North, FRotator(0.0,0.0,0.0)},
		};
	}
}

FString AMaze::ToJSONString() const
{
	const TSharedRef<FJsonObject> JsonRootObject = this->ToJSON();
	
	FString OutputString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonRootObject, Writer);

	return OutputString;
}

TSharedRef<FJsonObject> AMaze::ToJSON() const
{
	//TODO: We should memoize the maze node JSON since we don't need to recalculate it each time
	//TODO: Reset the memoized maze node JSON when the maze changes
	//TODO: Maybe store the memoized JSON in the ConfigureMaze function, since this is when it is changed
	TArray<TSharedPtr<FJsonValue>> Items;

	const TSharedRef<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject);

	for(std::vector<FMazeNode*> Row: this->Nodes)
	{
		for(const FMazeNode* Node: Row)
		{
			Items.Add(MakeShareable(new FJsonValueObject(Node->ToJSON())));
		}
	}

	JsonRootObject->SetArrayField("nodes", Items);
	JsonRootObject->SetNumberField("width", this->Width);
	JsonRootObject->SetNumberField("depth", this->Depth);
	JsonRootObject->SetNumberField("goal_x", this->End->Coordinates.X);
	JsonRootObject->SetNumberField("goal_y", this->End->Coordinates.Y);

	return JsonRootObject;
}

FString AMaze::GetPositionsString() const
{
	const TSharedRef<FJsonObject> JsonRootObject = this->PositionsJSON();
	
	FString OutputString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonRootObject, Writer);

	return OutputString;
}

TSharedRef<FJsonObject> AMaze::PositionsJSON() const
{
	const TSharedRef<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject);
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), VRPawn, FoundActors);
	const FMazeCoordinates PlayerPosition = this->WorldLocationToMazeCoordinates(FoundActors[0]->GetActorLocation());

	JsonRootObject->SetNumberField("player_x", PlayerPosition.X);
	JsonRootObject->SetNumberField("player_y", PlayerPosition.Y);
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MonsterBP, FoundActors);
	const FMazeCoordinates MonsterPosition = this->WorldLocationToMazeCoordinates(FoundActors[0]->GetActorLocation());

	JsonRootObject->SetNumberField("monster_x", MonsterPosition.X);
	JsonRootObject->SetNumberField("monster_y", MonsterPosition.Y);

	return JsonRootObject;
}

void AMaze::InitialiseNodes()
{
	// Initialise our nodes 2D vector
	for(float i = 0.0f; i < this->Width; i++)
	{
		std::vector<FMazeNode*> Row;
		for(float j = 0.0f; j < this->Depth; j++)
		{
			Row.push_back(new FMazeNode(this, FMazeCoordinates{j, i}, FNodeExits{}));
		}
		Nodes.push_back(Row);
	}
}

void AMaze::ConfigureMaze(FMazeGenerator* Generator)
{
	Generator->GenerateMaze(this);

	// Move the player to the start of the maze
	FVector MazeStartPosition = MazeCoordinatesToWorldLocation(Start->Coordinates);
	
	MazeStartPosition += FVector{0, 0, 500};

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), VRPawn, FoundActors);
	
	FoundActors[0]->SetActorLocation(MazeStartPosition);

	// Spawn the goal at the end of the maze
	const std::pair<FMazeCoordinates,FRotator> GoalPair = this->GetPositionAndRotationForGoal();
	
	GetWorld()->SpawnActor<AActor>(
		GoalBP,
		MazeCoordinatesToWorldLocation(GoalPair.first),
		GoalPair.second
	);

	// Spawn the monster somewhere in the maze
	const std::pair<FMazeNode*,std::pair<EMaze_Direction, FRotator>> MonsterPair = GetMonsterStartingPositionAndRotation();
	
	AActor* Monster = GetWorld()->SpawnActor<AActor>(
		MonsterBP,
		MazeCoordinatesToWorldLocation(MonsterPair.first->Coordinates) + FVector{0, 0, this->MonsterPositionHeight},
		MonsterPair.second.second
	);

	AMonster* SpawnedMonster = static_cast<AMonster*>(Monster);
	SpawnedMonster->DirectionChanged(MonsterPair.second.first);
	SpawnedMonster->GoalNode = MonsterPair.first;
}

void AMaze::SpawnMazeGridBPs() const
{
	for (std::vector<FMazeNode*> Row : Nodes)
	{
		for(const FMazeNode* Node : Row)
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(
				this->GetActorForExits(Node->Exits),
				this->MazeCoordinatesToWorldLocation(Node->Coordinates),
				this->GetRotationForExits(Node->Exits)
			);

			Actor->SetActorRelativeScale3D(FVector(BlueprintScale));
		}
	}
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();

	this->InitialiseNodes();
	
	SimplePrimMaze g = SimplePrimMaze();
	this->ConfigureMaze(&g);
	
	this->SpawnMazeGridBPs();
	
	SC = new ServerSocketClient();
	
	// Tell the server about this maze
	SC->SendStartCommand(MazeCode, this);
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SC->SendPositions(this);

	const FString Message = SC->ReadMessage();

	UE_LOG(LogClass, Log, TEXT("Message: %s"), *Message);
	
	if(Message.StartsWith(MazeCode + " MonsterDirection "))
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), MonsterBP, FoundActors);
		AMonster* Monster = static_cast<AMonster*>(FoundActors[0]);
		const FString Direction =  Message.Mid(22);
		UE_LOG(LogClass, Log, TEXT("Message: \"%s\""), *Direction);

		//TODO: cannot use a switch statement on FString
		if (Direction.Equals(FString("north")))
		{
			Monster->DirectionChanged(North);
		}else if(Direction.Equals( FString("east")))
		{
			Monster->DirectionChanged(East);
		}
		else if(Direction.Equals( FString("south")))
		{
			Monster->DirectionChanged(South);
		}
		else if(Direction.Equals( FString("west")))
		{
			Monster->DirectionChanged(West);
		}
		else
		{
			// Can't throw an exception here because Unreal Engine does not
			// support building them to Android applications
			GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::White,  FString(
				TEXT("Recieved an unkown MonsterDirection message: ") + Message));
		}
	}
}
