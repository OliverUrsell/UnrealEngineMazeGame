#pragma once

#include "CoreMinimal.h"
#include "StartMenuFunctions.generated.h"

UCLASS(Blueprintable)
class MAZEGAMETRY2_API UStartMenuFunctions final : public UObject
{
  GENERATED_BODY()
  
public:
  UFUNCTION(BlueprintCallable)
  static bool ConnectToServer();

  UFUNCTION(BlueprintCallable)
  static bool IsConnected();

  UFUNCTION(BlueprintCallable)
  static FString GetCode();
};
