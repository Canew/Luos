

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LuosGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ALuosGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GameMode")
	class AGameSession* GetGameSession() const;

	void ProcessPlayerDeath(APlayerController* Player);
};
