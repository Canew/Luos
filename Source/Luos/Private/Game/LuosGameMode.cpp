


#include "Game/LuosGameMode.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "Character/LuosCharacter.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosGameMode)

AGameSession* ALuosGameMode::GetGameSession() const
{
	return GameSession;
}

void ALuosGameMode::ProcessPlayerDeath(APlayerController* Player)
{
	if (Player->PlayerState)
	{
		Player->PlayerState->SetIsSpectator(true);
	}

	Player->ChangeState(NAME_Spectating);
	Player->ClientGotoState(NAME_Spectating);
	Player->ServerSetSpectatorWaiting(true);
	Player->ClientSetSpectatorWaiting(true);

	bool bFoundAliveCharacter = false;
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		ALuosCharacter* PlayerCharacter = PlayerController->GetPawn<ALuosCharacter>();
		if (IsValid(PlayerCharacter) && PlayerCharacter->IsAlive())
		{
			FTimerHandle TimerHandle;
			Player->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
				[Player, PlayerCharacter]() -> void
				{
					if (IsValid(Player))
					{
						Player->SetViewTarget(PlayerCharacter);
						Player->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0f, 0.0f), 0.5f);
					}
				}), 1.0f, false);

			bFoundAliveCharacter = true;
			break;
		}
	}

	if (!bFoundAliveCharacter)
	{
		FTimerHandle RestartTimerHandle;
		float RestartDelay = 2.0f;
		GetWorldTimerManager().SetTimer(RestartTimerHandle, FTimerDelegate::CreateLambda(
			[this]() -> void
			{
			GetWorld()->ServerTravel("?Restart", false);
			}), RestartDelay, false);
	}
}
