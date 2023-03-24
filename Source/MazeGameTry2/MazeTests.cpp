
#include "AITestsCommon.h"
#include "Maze.h"
#include "MockServerSocketClient.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationCommon.h"

BEGIN_DEFINE_SPEC(MazeTest, "MazeGameTry2.Maze", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
// Global variables that are shared by all tests go here
UWorld* World = nullptr;
AMaze* Maze;
END_DEFINE_SPEC(MazeTest)

static UWorld* GetWorld()
{
    if (GEngine)
    {
        if (FWorldContext* WorldContext = GEngine->GetWorldContextFromPIEInstance(0))
        {
            return WorldContext->World();
        }
    }
    return nullptr;
}

static void Exit()
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* TargetPC = UGameplayStatics::GetPlayerController(World, 0))
        {
            TargetPC->ConsoleCommand(TEXT("Exit"), true);
        }
    }
}

// Tests in this function will be executed. Generally, you only need this function to run test. Class is not needed.
void MazeTest::Define()
{
    // Code to run before each test
    BeforeEach([this]()
    {
        UE_LOG(LogTemp, Display, TEXT("Here in the test method -3"));
        // Create a mock socket so we don't need to connect to a real server
        MockServerSocketClient();

        UE_LOG(LogTemp, Display, TEXT("Here in the test method -2"));
        
        // Open the game map to test with
        AutomationOpenMap("/Game/VRTemplate/Maps/VRTemplateMap.VRTemplateMap");
        TestNotNull("Check if World is properly created", World);
        // Pointer to created world
        World = GetWorld();

        // Get the Maze
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(World, AMaze::StaticClass(), FoundActors);
        Maze = static_cast<AMaze*>(FoundActors[0]);
        UE_LOG(LogTemp, Display, TEXT("Here in the test method -1"));
    });

    // Code to run after each test
    AfterEach([this]()
    {
        UE_LOG(LogTemp, Display, TEXT("Here in the test method 5"));
        Exit();
        UE_LOG(LogTemp, Display, TEXT("Here in the test method 6"));
        // Maze->Destroy();
    });

    Describe("Maze Generation", [this]()
    {
        It("Should decide on a start node", [this]()
        {
            UE_LOG(LogTemp, Display, TEXT("Here in the test method"));
            // TestNotNull("The Maze Start attribute should not be null", Maze->Start);
            TestEqual("Should be equal", 1, 1);
            UE_LOG(LogTemp, Display, TEXT("Here in the test method 2"));
            // TestEqual("ShootBullet should only creates one bullet", ShooterA->NumberOfBulletShot, 1);
            // TestTrue("bIsBulletShot should be changed to true", ShooterA->bIsBulletShot);
            // TestNotNull("Bullet reference in ShooterA should be valid", ShooterA->ShotBullet);
        });
        
        It("Should decide on a goal node", [this]()
        {
            UE_LOG(LogTemp, Display, TEXT("Here in the test method 3"));
            // TestNotNull("The Maze End attribute should not be null", Maze->End);
            TestEqual("Should be equal 2", 2, 1);
            UE_LOG(LogTemp, Display, TEXT("Here in the test method 4"));
            // TestEqual("ShootBullet should only creates one bullet", ShooterA->NumberOfBulletShot, 1);
            // TestTrue("bIsBulletShot should be changed to true", ShooterA->bIsBulletShot);
            // TestNotNull("Bullet reference in ShooterA should be valid", ShooterA->ShotBullet);
        });
    });
    
    // Describe("Shoot Skill Function", [this]()
    // {
    //     /** Latent function - Function that runs for few ticks
    //      *  Sometimes your function needs to runs for few frames, latent test can be used for its test
    //      *  Example: Skill that tells player how many enemies still alive
    //      *  Similar to `It` function with extra parameter that let you do something in another thread
    //      *  Generally I just do it in GameThread
    //      *  FDoneDelegate YourVariableName : basically just for the engine to know if the latent test finishes the job
    //      */
    //     LatentIt("HealthCheckSkill from Shoot Skill", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
    //     {
    //         // Because latent test runs on a separate thread we have to ensure that game logic tests run on a Game Thread. 
    //         AsyncTask(ENamedThreads::GameThread, [this]() 
    //         {
    //             ShooterA->UseSkill();
    //
    //             TestTrue("Something something", ShooterA->bUsedSkill);
    //             // Some other test
    //         });
    //
    //         // Wait for X second. Let's say 1.0f second.
    //         FPlatformProcess::Sleep(1.0f);
    //
    //         // Again, GameThread
    //         AsyncTask(ENamedThreads::GameThread, [this]() 
    //         {
    //             // You may do something here
    //             // In this example, I will straight jump to the test part
    //             TestEqual("X number of enemies left.", ShooterA->NumberOfEnemiesLeft, X);
    //         });
    //
    //         // This will delegate to inform Unreal Automation System that the latent function has finished.
    //         // So it will run other stuff. If I am not wrong, `latent functions` normally will only run after all the non-latent functions.
    //         TestDone.Execute();
    //     });
    // });
}