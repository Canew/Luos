


#include "Game/LuosGameInstance.h"
#include "GameFramework/GameUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosGameInstance)

void ULuosGameInstance::Init()
{
	Super::Init();

	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (GameUserSettings)
	{
		GameUserSettings->SetOverallScalabilityLevel(1);
		GameUserSettings->ApplySettings(true);
	}
}

FString ULuosGameInstance::GetMainMenuLevelURL() const
{
	return TEXT("/Game/Maps/MainMenu/MainMenu");
}

FString ULuosGameInstance::GetLobbyLevelURL() const
{
	return TEXT("/Game/Maps/Lobby/Lobby");
}

FString ULuosGameInstance::GetGameplayLevelURL() const
{
	return TEXT("/Game/Maps/Forest/Forest");
}
