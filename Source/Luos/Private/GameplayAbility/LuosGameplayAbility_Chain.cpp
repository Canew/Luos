


#include "GameplayAbility/LuosGameplayAbility_Chain.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "GameplayAbility/LuosAbilityTask_WaitAbilityEnd.h"
#include "GameplayAbility/LuosGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosGameplayAbility_Chain)

ULuosGameplayAbility_Chain::ULuosGameplayAbility_Chain()
{
	CurrentChainAbilityIndex = 0;
	MaxChainInterval = 0.0f;
	bCanActivateChainAbility = true;
}

bool ULuosGameplayAbility_Chain::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!IsActive())
	{
		if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		{
			return false;
		}
	}

	return CanActivateChainAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

float ULuosGameplayAbility_Chain::GetMaxChainInterval() const
{
	return MaxChainInterval;
}

float ULuosGameplayAbility_Chain::GetRemainingChainDuration() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (World->GetTimerManager().TimerExists(ChainTimerHandle))
		{
			return World->GetTimerManager().GetTimerRemaining(ChainTimerHandle);
		}
	}

	return 0.0f;
}

void ULuosGameplayAbility_Chain::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void ULuosGameplayAbility_Chain::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActivateChainAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void ULuosGameplayAbility_Chain::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentChainAbilityIndex = 0;
	bCanActivateChainAbility = true;
}

bool ULuosGameplayAbility_Chain::CanActivateChainAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!bCanActivateChainAbility || !ChainAbilities.IsValidIndex(CurrentChainAbilityIndex))
	{
		return false;
	}

	FGameplayAbilitySpec* ChainAbilitySpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromClass(ChainAbilities[CurrentChainAbilityIndex]);
	if (ChainAbilitySpec)
	{
		if (ULuosGameplayAbility* LuosGameplayAbility = Cast<ULuosGameplayAbility>(ChainAbilitySpec->Ability))
		{
			return LuosGameplayAbility->CanActivateAbility(ChainAbilitySpec->Handle, ActorInfo);
		}
	}

	return false;
}

bool ULuosGameplayAbility_Chain::ActivateChainAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	bool bSucceeded = false;
	if (ChainAbilities.IsValidIndex(CurrentChainAbilityIndex))
	{
		bSucceeded = GetCurrentActorInfo()->AbilitySystemComponent->TryActivateAbilityByClass(ChainAbilities[CurrentChainAbilityIndex]);

		bCanActivateChainAbility = false;

		WaitAbilityEndTask = ULuosAbilityTask_WaitAbilityEnd::WaitForAbilityEndWithClass(this, ChainAbilities[CurrentChainAbilityIndex]);
		if (WaitAbilityEndTask)
		{
			WaitAbilityEndTask->OnAbilityEnd.AddDynamic(this, &ThisClass::HandleAbilityEnded);
			WaitAbilityEndTask->ReadyForActivation();
		}

		WaitInputPress();

		CurrentChainAbilityIndex++;
	}

	return bSucceeded;
}

void ULuosGameplayAbility_Chain::HandleInputPress(float TimeWaited)
{
	if (CanActivateChainAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()))
	{
		if (WaitAbilityEndTask && WaitAbilityEndTask->IsActive())
		{
			WaitAbilityEndTask->EndTask();
		}

		if (GetCurrentActorInfo()->AbilitySystemComponent->GetWorld()->GetTimerManager().TimerExists(ChainTimerHandle))
		{
			GetCurrentActorInfo()->AbilitySystemComponent->GetWorld()->GetTimerManager().ClearTimer(ChainTimerHandle);
		}

		FGameplayAbilitySpec* GameplayAbilitySpec = GetCurrentActorInfo()->AbilitySystemComponent->FindAbilitySpecFromClass(ChainAbilities[CurrentChainAbilityIndex - 1]);
		if (GameplayAbilitySpec)
		{
			if (UGameplayAbility* GameplayAbility = GameplayAbilitySpec->GetPrimaryInstance())
			{
				GameplayAbility->CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false);
			}
		}

		ActivateChainAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), nullptr);
	}
	else
	{
		WaitInputPress();
	}
}

void ULuosGameplayAbility_Chain::HandleAbilityEnded(UGameplayAbility* Ability)
{
	if (CurrentChainAbilityIndex == ChainAbilities.Num() || MaxChainInterval == 0.0f)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else if (bCanActivateChainAbility && CurrentChainAbilityIndex < ChainAbilities.Num())
	{
		GetCurrentActorInfo()->AbilitySystemComponent->GetWorld()->GetTimerManager().SetTimer(ChainTimerHandle, this, &ThisClass::HandleChainTimerElapsed, MaxChainInterval, false);
	}
	else
	{
		WaitInputPress();
	}
}

void ULuosGameplayAbility_Chain::WaitInputPress()
{
	WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	if (WaitInputPressTask)
	{
		WaitInputPressTask->OnPress.AddDynamic(this, &ThisClass::HandleInputPress);
		WaitInputPressTask->ReadyForActivation();

		bCanActivateChainAbility = true;
	}
}

void ULuosGameplayAbility_Chain::HandleChainTimerElapsed()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
