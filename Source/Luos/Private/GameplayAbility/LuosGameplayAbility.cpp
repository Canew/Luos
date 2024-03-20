


#include "GameplayAbility/LuosGameplayAbility.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "GameplayAbility/LuosGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosGameplayAbility)

ULuosGameplayAbility::ULuosGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool ULuosGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!IsValid(AbilitySystemComponent))
	{
		return false;
	}

	bool bBlocked = false;
	switch (ActivationGroup)
	{
	case ELuosAbilityActivationGroup::Independent:
		break;

	case ELuosAbilityActivationGroup::Exclusive_Replaceable:
	case ELuosAbilityActivationGroup::Exclusive_Blocking:
	{
		TArray<FGameplayAbilitySpec> ActivatableAbilitySpecs = AbilitySystemComponent->GetActivatableAbilities();
		for (FGameplayAbilitySpec& ActivatableAbilitySpec : ActivatableAbilitySpecs)
		{
			if (ActivatableAbilitySpec.IsActive() && !ActivatableAbilitySpec.DynamicAbilityTags.HasTagExact(LuosGameplayTags::Ability_Property_Cancellable))
			{
				if (ULuosGameplayAbility* LuosGameplayAbility = Cast<ULuosGameplayAbility>(ActivatableAbilitySpec.Ability))
				{
					if (LuosGameplayAbility->GetActivationGroup() == ELuosAbilityActivationGroup::Exclusive_Blocking)
					{
						bBlocked = true;
						break;
					}
				}
			}
		}
		break;
	}

	default:
		return false;
	}

	if (bBlocked)
	{
		return false;
	}

	return true;
}

void ULuosGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	if (TriggerEventData)
	{
		CurrentEventData = *TriggerEventData;
	}
}

void ULuosGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* AbilitySystenComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystenComponent || !HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo) || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Grant effects
	for (FLuosGrantedGameplayEffectDef GrantedEffect : GrantedEffects)
	{
		if (GrantedEffect.EffectActivationPolicy == FLuosGrantedEffectActivationPolicy::OnActivated)
		{
			GrantedEffect.ActiveEffectHandle = AbilitySystenComponent->ApplyGameplayEffectToSelf(GrantedEffect.EffectClass.GetDefaultObject(), 0.0f, MakeEffectContext(Handle, ActorInfo), AbilitySystenComponent->ScopedPredictionKey);
		}
		else if (GrantedEffect.EffectActivationPolicy == FLuosGrantedEffectActivationPolicy::NotOnActivated)
		{
			FGameplayEffectQuery EffectQuery;
			EffectQuery.EffectDefinition = GrantedEffect.EffectClass;
			AbilitySystenComponent->RemoveActiveEffects(EffectQuery);
		}
	}

	// Gameplay events
	if (bWaitCancellableEvent)
	{
		FGameplayEventMulticastDelegate& CancellableEventMulticastDelegate = AbilitySystenComponent->GenericGameplayEventCallbacks.FindOrAdd(LuosGameplayTags::GameplayEvent_Cancellable);
		if (!CancellableEventMulticastDelegate.IsBoundToObject(this))
		{
			CancellableEventMulticastDelegateHandle = CancellableEventMulticastDelegate.AddUObject(this, &ThisClass::CancellableGameplayEventCallback);
		}
	}
}

void ULuosGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ULuosAbilitySystemComponent* AbilitySystenComponent = GetLuosAbilitySystemComponentFromActorInfo();
	if (AbilitySystenComponent && HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		AbilitySystenComponent->InjectRecentInputForAction();

		// Remove cost effect
		if (CostGameplayEffectClass)
		{
			UGameplayEffect* CostGameplayEffect = CostGameplayEffectClass.GetDefaultObject();
			if (CostGameplayEffect->DurationPolicy == EGameplayEffectDurationType::Infinite)
			{
				AbilitySystenComponent->RemoveActiveGameplayEffectBySourceEffect(CostGameplayEffectClass, AbilitySystenComponent);
			}
		}

		// Grant effects
		for (FLuosGrantedGameplayEffectDef GrantedEffect : GrantedEffects)
		{
			if (GrantedEffect.EffectActivationPolicy == FLuosGrantedEffectActivationPolicy::OnActivated)
			{
				FGameplayEffectQuery EffectQuery;
				EffectQuery.EffectDefinition = GrantedEffect.EffectClass;
				AbilitySystenComponent->RemoveActiveEffects(EffectQuery);
			}
			else if (GrantedEffect.EffectActivationPolicy == FLuosGrantedEffectActivationPolicy::NotOnActivated)
			{
				GrantedEffect.ActiveEffectHandle = AbilitySystenComponent->ApplyGameplayEffectToSelf(GrantedEffect.EffectClass.GetDefaultObject(), 0.0f, MakeEffectContext(Handle, ActorInfo), AbilitySystenComponent->ScopedPredictionKey);
			}
		}

		// Gameplay events
		if (bWaitCancellableEvent)
		{
			FGameplayEventMulticastDelegate& CancellableGameplayEventMulticastDelegate = AbilitySystenComponent->GenericGameplayEventCallbacks.FindOrAdd(LuosGameplayTags::GameplayEvent_Cancellable);
			if (CancellableGameplayEventMulticastDelegate.IsBoundToObject(this))
			{
				CancellableGameplayEventMulticastDelegate.Remove(CancellableEventMulticastDelegateHandle);
			}

			FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
			if (AbilitySpec)
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(LuosGameplayTags::Ability_Property_Cancellable);
			}
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

const FGameplayTagContainer* ULuosGameplayAbility::GetCooldownTags() const
{
	if (!CooldownGameplayEffectClass)
	{
		return Super::GetCooldownTags();
	}

	// Get cooldown tag from input tag
	FGameplayTagContainer* CooldownTags = const_cast<FGameplayTagContainer*>(&CooldownTagsContainer);
	CooldownTags->Reset();
	const FGameplayTagContainer* ParentCooldownTags = Super::GetCooldownTags();
	if (ParentCooldownTags)
	{
		CooldownTags->AppendTags(*ParentCooldownTags);
	}

	if (FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec())
	{
		FGameplayTagContainer InputTags;
		InputTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Input")));
		InputTags = AbilitySpec->DynamicAbilityTags.Filter(InputTags);

		CooldownTags->AppendTags(InputTags);
	}

	return CooldownTags;
}

void ULuosGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	const FGameplayTagContainer* CooldownTags = GetCooldownTags();
	if (CooldownGE && CooldownTags)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(*CooldownTags);

		SpecHandle.Data.Get()->SetSetByCallerMagnitude(LuosGameplayTags::Data_Cooldown, CooldownTime);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

ULuosAbilitySystemComponent* ULuosGameplayAbility::GetLuosAbilitySystemComponentFromActorInfo() const
{
	return Cast<ULuosAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

int32 ULuosGameplayAbility::GetPredictionKey() const
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		return ASC->ScopedPredictionKey.Current;
	}

	return 0;
}

FGameplayEffectSpecHandle ULuosGameplayAbility::MakeGameplayEffectSpecFromHitResult(TSubclassOf<UGameplayEffect> GameplayEffectClass, float GameplayEffectLevel, const FHitResult& HitResult)
{
	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();

	FGameplayEffectContextHandle GameplayEffectContextHandle = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
	GameplayEffectContextHandle.AddHitResult(HitResult, true);
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, GameplayEffectLevel, GameplayEffectContextHandle);
	if (GameplayEffectSpecHandle.IsValid())
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle);
		ApplyAbilityTagsToGameplayEffectSpec(*GameplayEffectSpecHandle.Data.Get(), AbilitySpec);

		// Copy over set by caller magnitudes
		if (AbilitySpec)
		{
			GameplayEffectSpecHandle.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;
		}
	}

	return GameplayEffectSpecHandle;
}

void ULuosGameplayAbility::CancellableGameplayEventCallback(const FGameplayEventData* Payload)
{
	ULuosAbilitySystemComponent* AbilitySystemComponent = Cast<ULuosAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* GameplayAbilitySpec = GetCurrentAbilitySpec();
		if (GameplayAbilitySpec)
		{
			GameplayAbilitySpec->DynamicAbilityTags.AddTag(LuosGameplayTags::Ability_Property_Cancellable);

			AbilitySystemComponent->InjectRecentInputForAction();
		}
	}
}
