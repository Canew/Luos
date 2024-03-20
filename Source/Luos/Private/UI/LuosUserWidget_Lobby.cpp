


#include "UI/LuosUserWidget_Lobby.h"
#include "CommonButtonBase.h"
#include "Game/LuosGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULuosUserWidget_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked().AddUObject(this, &ThisClass::StartButtonClickedCallback);
	QuitButton->OnClicked().AddUObject(this, &ThisClass::QuitButtonClickedCallback);
}

void ULuosUserWidget_Lobby::StartButtonClickedCallback()
{
	check(GetOwningPlayer());
	if (GetOwningPlayer()->HasAuthority())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ULuosGameInstance* GameInstance = World->GetGameInstance<ULuosGameInstance>();
			if (GameInstance)
			{
				World->ServerTravel(GameInstance->GetGameplayLevelURL());
			}
		}
	}
	else
	{
		// Client do ready
	}
}

void ULuosUserWidget_Lobby::QuitButtonClickedCallback()
{
	check(GetOwningPlayer());

	UWorld* World = GetWorld();
	if (World)
	{
		ULuosGameInstance* GameInstance = World->GetGameInstance<ULuosGameInstance>();
		if (GameInstance)
		{
			GetOwningPlayer()->ClientTravel(GameInstance->GetMainMenuLevelURL(), ETravelType::TRAVEL_Absolute);
		}
	}
}
