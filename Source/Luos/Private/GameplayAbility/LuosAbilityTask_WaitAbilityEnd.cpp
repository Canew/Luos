


#include "GameplayAbility/LuosAbilityTask_WaitAbilityEnd.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAbilityTask_WaitAbilityEnd)

ULuosAbilityTask_WaitAbilityEnd::ULuosAbilityTask_WaitAbilityEnd(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityClass = nullptr;
}

void ULuosAbilityTask_WaitAbilityEnd::Activate()
{
	if (AbilitySystemComponent.IsValid())
	{
		OnAbilityEndDelegateHandle = AbilitySystemComponent->AbilityEndedCallbacks.AddUObject(this, &ULuosAbilityTask_WaitAbilityEnd::HandleAbilityEnded);
	}
}

void ULuosAbilityTask_WaitAbilityEnd::HandleAbilityEnded(UGameplayAbility* EndedAbility)
{
	if (TagRequirements.IsEmpty())
	{
		if ((WithTag.IsValid() && !EndedAbility->AbilityTags.HasTag(WithTag)) ||
			(WithoutTag.IsValid() && EndedAbility->AbilityTags.HasTag(WithoutTag)))
		{
			// Failed tag check
			return;
		}
	}
	else
	{
		if (!TagRequirements.RequirementsMet(EndedAbility->AbilityTags))
		{
			// Failed tag check
			return;
		}
	}

	if (Query.IsEmpty() == false)
	{
		if (Query.Matches(EndedAbility->AbilityTags) == false)
		{
			// Failed query
			return;
		}
	}

	if (AbilityClass)
	{
		if (AbilityClass != EndedAbility->GetClass())
		{
			return;
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityEnd.Broadcast(EndedAbility);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}

ULuosAbilityTask_WaitAbilityEnd* ULuosAbilityTask_WaitAbilityEnd::WaitForAbilityEnd(UGameplayAbility* OwningAbility, FGameplayTag InWithTag, FGameplayTag InWithoutTag, bool InTriggerOnce)
{
	ULuosAbilityTask_WaitAbilityEnd* MyObj = NewAbilityTask<ULuosAbilityTask_WaitAbilityEnd>(OwningAbility);
	MyObj->WithTag = InWithTag;
	MyObj->WithoutTag = InWithoutTag;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

ULuosAbilityTask_WaitAbilityEnd* ULuosAbilityTask_WaitAbilityEnd::WaitForAbilityEndWithTagRequirements(UGameplayAbility* OwningAbility, FGameplayTagRequirements TagRequirements, bool InTriggerOnce)
{
	ULuosAbilityTask_WaitAbilityEnd* MyObj = NewAbilityTask<ULuosAbilityTask_WaitAbilityEnd>(OwningAbility);
	MyObj->TagRequirements = TagRequirements;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

ULuosAbilityTask_WaitAbilityEnd* ULuosAbilityTask_WaitAbilityEnd::WaitForAbilityEnd_Query(UGameplayAbility* OwningAbility, FGameplayTagQuery Query, bool InTriggerOnce)
{
	ULuosAbilityTask_WaitAbilityEnd* MyObj = NewAbilityTask<ULuosAbilityTask_WaitAbilityEnd>(OwningAbility);
	MyObj->Query = Query;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

ULuosAbilityTask_WaitAbilityEnd* ULuosAbilityTask_WaitAbilityEnd::WaitForAbilityEndWithClass(UGameplayAbility* OwningAbility, TSubclassOf<UGameplayAbility> AbilityClass, bool InTriggerOnce)
{
	ULuosAbilityTask_WaitAbilityEnd* MyObj = NewAbilityTask<ULuosAbilityTask_WaitAbilityEnd>(OwningAbility);
	MyObj->AbilityClass = AbilityClass;
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;

}

void ULuosAbilityTask_WaitAbilityEnd::OnDestroy(bool AbilityEnded)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityEndedCallbacks.Remove(OnAbilityEndDelegateHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

