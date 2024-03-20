


#include "GameplayAbility/RaveAbilityTask_TurnTowards.h"
#include "Character/LuosCharacter.h"

URaveAbilityTask_TurnTowards::URaveAbilityTask_TurnTowards()
{
	bTickingTask = true;
}

URaveAbilityTask_TurnTowards* URaveAbilityTask_TurnTowards::TurnTowards(UGameplayAbility* OwningAbility, FVector Direction, float CompleteThreshold, bool bTurnInstantly, bool bEndTaskAfterTurn)
{
	URaveAbilityTask_TurnTowards* MyObj = NewAbilityTask<URaveAbilityTask_TurnTowards>(OwningAbility);
	MyObj->Direction = Direction;
	MyObj->CompleteThreshold = CompleteThreshold;
	MyObj->bTurnInstantly = bTurnInstantly;
	MyObj->bEndTaskAfterTurn = bEndTaskAfterTurn;

	return MyObj;
}

void URaveAbilityTask_TurnTowards::Activate()
{
	Super::Activate();

	SetWaitingOnAvatar();

	AvatarCharacter = Cast<ALuosCharacter>(GetAvatarActor());
	if (!IsValid(AvatarCharacter.Get()))
	{
		EndTask();

		return;
	}

	Direction.Z = 0.0f;
	Direction.Normalize();

	CompleteThreshold = FMath::Clamp(CompleteThreshold, KINDA_SMALL_NUMBER, CompleteThreshold);
}

void URaveAbilityTask_TurnTowards::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!IsValid(AvatarCharacter.Get()))
	{
		EndTask();
	}

	AvatarCharacter->Turn(Direction, bTurnInstantly);

	if (FMath::Abs(AvatarCharacter->GetActorForwardVector().Rotation().Yaw - Direction.Rotation().Yaw) < CompleteThreshold)
	{
		OnCompleted.Broadcast();

		if (bEndTaskAfterTurn)
		{
			EndTask();
		}
	}
}
