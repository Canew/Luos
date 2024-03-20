


#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "GameplayAbility/LuosAttributeSet.h"
#include "GameplayAbility/LuosGameplayTags.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAbilitySystemComponent)

ULuosAbilitySystemComponent::ULuosAbilitySystemComponent()
{
	ActionBufferDuration = 0.3f;
}

void ULuosAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void ULuosAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void ULuosAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			const ULuosGameplayAbility* LuosAbilityCDO = CastChecked<ULuosGameplayAbility>(AbilitySpec->Ability);
			if (LuosAbilityCDO->GetActivationPolicy() == ELuosAbilityActivationPolicy::WhileInputActive && LuosAbilityCDO->GetActivationPolicy() != ELuosAbilityActivationPolicy::OnEventReceived)
			{
				AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			AbilitySpec->InputPressed = true;

			const ULuosGameplayAbility* LuosAbilityCDO = CastChecked<ULuosGameplayAbility>(AbilitySpec->Ability);
			if (AbilitySpec->IsActive())
			{
				// Ability is active so pass along the input event.
				AbilitySpecInputPressed(*AbilitySpec);
				AbilityInstanceInputPressed(*AbilitySpec);
			}

			if (LuosAbilityCDO->GetActivationPolicy() == ELuosAbilityActivationPolicy::OnInputTriggered && LuosAbilityCDO->GetActivationPolicy() != ELuosAbilityActivationPolicy::OnEventReceived)
			{
				AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (int32 AbilitySpecHandleIndex = 0; AbilitySpecHandleIndex < AbilitiesToActivate.Num(); AbilitySpecHandleIndex++)
	{
		if (TryActivateAbility(AbilitiesToActivate[AbilitySpecHandleIndex]))
		{
			// Clear action buffer when the ability succeeded to activate.
			SetRecentInputAction(FGameplayAbilitySpecHandle());
		}
		else
		{
			// Push last pressed ability to action buffer if failed to try activate.
			if (AbilitySpecHandleIndex == AbilitiesToActivate.Num() - 1)
			{
				SetRecentInputAction(AbilitiesToActivate[AbilitySpecHandleIndex]);
			}
		}
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
					AbilityInstanceInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void ULuosAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

bool ULuosAbilitySystemComponent::CanApplyGameplayEffect(const FGameplayEffectSpecHandle& EffectSpecHandle) const
{
	if (EffectSpecHandle.Data && EffectSpecHandle.Data->Def)
	{
		return EffectSpecHandle.Data->Def->CanApply(ActiveGameplayEffects, *EffectSpecHandle.Data);
	}

	return false;
}

void ULuosAbilitySystemComponent::CancelAbilityClass(TSubclassOf<UGameplayAbility> InAbilityToCancel)
{
	const UGameplayAbility* const InAbilityCDO = InAbilityToCancel.GetDefaultObject();

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability == InAbilityCDO)
		{
			CancelAbilityHandle(Spec.Handle);
			break;
		}
	}
}

void ULuosAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	// Granted effects
	if (ULuosGameplayAbility* LuosGameplayAbility = Cast<ULuosGameplayAbility>(AbilitySpec.Ability))
	{
		for (FLuosGrantedGameplayEffectDef& GrantedEffect : LuosGameplayAbility->GetGrantedEffects())
		{
			if (GrantedEffect.EffectActivationPolicy == FLuosGrantedEffectActivationPolicy::Always
				|| GrantedEffect.EffectActivationPolicy == FLuosGrantedEffectActivationPolicy::NotOnActivated)
			{
				GrantedEffect.ActiveEffectHandle = ApplyGameplayEffectToSelf(GrantedEffect.EffectClass.GetDefaultObject(), 1.0f, MakeEffectContext(), ScopedPredictionKey);
			}
		}
	}

	OnAbilityAdded.Broadcast(AbilitySpec);
}

void ULuosAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);

	// Granted effects
	if (ULuosGameplayAbility* LuosGameplayAbility = Cast<ULuosGameplayAbility>(AbilitySpec.Ability))
	{
		for (FLuosGrantedGameplayEffectDef& GrantedEffect : LuosGameplayAbility->GetGrantedEffects())
		{
			if (GrantedEffect.bClearOnAbilityRemoved)
			{
				FGameplayEffectQuery EffectQuery;
				EffectQuery.EffectDefinition = GrantedEffect.EffectClass;
				RemoveActiveEffects(EffectQuery, 1);
			}
		}
	}

	OnAbilityRemoved.Broadcast(AbilitySpec);
}

void ULuosAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void ULuosAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void ULuosAbilitySystemComponent::AbilityInstanceInputPressed(FGameplayAbilitySpec& Spec)
{
	TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();
	for (UGameplayAbility* AbilityInstance : AbilityInstances)
	{
		AbilityInstance->InputPressed(Spec.Handle, AbilityActorInfo.Get(), AbilityInstance->GetCurrentActivationInfo());
	}
}

void ULuosAbilitySystemComponent::AbilityInstanceInputReleased(FGameplayAbilitySpec& Spec)
{
	TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();
	for (UGameplayAbility* AbilityInstance : AbilityInstances)
	{
		AbilityInstance->InputReleased(Spec.Handle, AbilityActorInfo.Get(), AbilityInstance->GetCurrentActivationInfo());
	}
}

void ULuosAbilitySystemComponent::SetRecentInputAction(const FGameplayAbilitySpecHandle& NewRecentInputAbilitySpecHandle)
{
	RecentInputAbilitySpecHandle = NewRecentInputAbilitySpecHandle;
	GetWorld()->GetTimerManager().SetTimer(ActionBufferTimerHandle, this, &ULuosAbilitySystemComponent::ClearRecentInputAction, ActionBufferDuration, false);
}

void ULuosAbilitySystemComponent::InjectRecentInputForAction()
{
	if (FGameplayAbilitySpec* GameplayAbilitySpec = FindAbilitySpecFromHandle(RecentInputAbilitySpecHandle))
	{
		ULuosGameplayAbility* LuosGameplayAbility = Cast<ULuosGameplayAbility>(GameplayAbilitySpec->GetPrimaryInstance());
		if (LuosGameplayAbility && LuosGameplayAbility->CanActivateAbility(GameplayAbilitySpec->Handle, AbilityActorInfo.Get()))
		{
			InputPressedSpecHandles.AddUnique(RecentInputAbilitySpecHandle);
			InputHeldSpecHandles.AddUnique(RecentInputAbilitySpecHandle);
			GetWorld()->GetTimerManager().ClearTimer(ActionBufferTimerHandle);
			ClearRecentInputAction();
		}
	}
}

void ULuosAbilitySystemComponent::ClearRecentInputAction()
{
	RecentInputAbilitySpecHandle = FGameplayAbilitySpecHandle();
}
