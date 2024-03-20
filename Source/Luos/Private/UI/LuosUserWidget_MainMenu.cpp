


#include "UI/LuosUserWidget_MainMenu.h"
#include "CommonButtonBase.h"
#include "Game/LuosGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULuosUserWidget_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked().AddUObject(this, &ThisClass::StartButtonClickedCallback);
	QuitButton->OnClicked().AddUObject(this, &ThisClass::QuitButtonClickedCallback);
}

void ULuosUserWidget_MainMenu::StartButtonClickedCallback()
{
	check(GetOwningPlayer());

	UWorld* World = GetWorld();
	if (World)
	{
		ULuosGameInstance* GameInstance = World->GetGameInstance<ULuosGameInstance>();
		if (GameInstance)
		{
			GetOwningPlayer()->ClientTravel(GameInstance->GetLobbyLevelURL(), ETravelType::TRAVEL_Absolute);
		}
	}
}

void ULuosUserWidget_MainMenu::QuitButtonClickedCallback()
{
	check(GetOwningPlayer());

	GetOwningPlayer()->ConsoleCommand("quit");
}
