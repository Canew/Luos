

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "AdvancedFriendsGameInstance.h"
#include "LuosGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	FString GetMainMenuLevelURL() const;
	FString GetLobbyLevelURL() const;
	FString GetGameplayLevelURL() const;
};
