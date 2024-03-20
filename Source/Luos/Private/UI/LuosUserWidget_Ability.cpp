


#include "UI/LuosUserWidget_Ability.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "GameplayAbility/LuosGameplayAbility.h"
#include "GameplayAbility/LuosAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosUserWidget_Ability)

void ULuosUserWidget_Ability::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeAbilitySystemComponent();
}

void ULuosUserWidget_Ability::InitializeAbilitySystemComponent()
{
	bool bSucceeded = false;
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwningPlayerState());
	if (AbilitySystemInterface)
	{
		AbilitySystemComponent = Cast<ULuosAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
		if (IsValid(AbilitySystemComponent.Get()))
		{
			InitializeAbilities();

			AbilitySystemComponent->OnAbilityAdded.AddUObject(this, &ThisClass::AbilityAddedCallback);
			AbilitySystemComponent->OnAbilityRemoved.AddUObject(this, &ThisClass::AbilityRemovedCallback);

			bSucceeded = true;
		}
	}

	if (!bSucceeded)
	{
		GetOwningPlayer()->GetWorldTimerManager().SetTimer(RequestAbilitySystemComponentTimerHandle, this, &ThisClass::InitializeAbilitySystemComponent, 1.0f, false);
	}
}

void ULuosUserWidget_Ability::InitializeAbilities()
{
	bool bFoundAbility = false;
	if (IsValid(AbilitySystemComponent.Get()))
	{
		for (const FGameplayAbilitySpec& ActivatableAbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if (ActivatableAbilitySpec.Ability && ActivatableAbilitySpec.DynamicAbilityTags.HasTag(AbilityInputTag))
			{
				CurrentAbilitySpec = ActivatableAbilitySpec;

				// Ability image
				ULuosGameplayAbility* ActivatableAbility = Cast<ULuosGameplayAbility>(CurrentAbilitySpec.Ability);
				AbilityImage->SetBrushFromTexture(ActivatableAbility->GetAbilityDescription().Icon);

				// Ability cost text block
				float AbilityCostMagnitude = 0.0f;
				UGameplayEffect* CostGameplayEffect = ActivatableAbility->GetCostGameplayEffect();
				if (CostGameplayEffect)
				{
					for (const FGameplayModifierInfo& GameplayModifierInfo : CostGameplayEffect->Modifiers)
					{
						if (GameplayModifierInfo.Attribute == ULuosAttributeSet::GetManaAttribute())
						{
							FGameplayEffectSpec CostGameplayEffectSpec = FGameplayEffectSpec(CostGameplayEffect, AbilitySystemComponent->MakeEffectContext());
							GameplayModifierInfo.ModifierMagnitude.AttemptCalculateMagnitude(CostGameplayEffectSpec, AbilityCostMagnitude);
						}
					}
				}
				AbilityCostMagnitude = FMath::Abs(AbilityCostMagnitude);
				AbilityCostTextBlock->SetText(FText::FromString(FString::SanitizeFloat(AbilityCostMagnitude, 0)));

				// Ability cost check
				if (ActivatableAbility->CheckCost(CurrentAbilitySpec.Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
				{
					AbilityBorder->SetIsEnabled(true);
				}
				else
				{
					AbilityBorder->SetIsEnabled(false);
				}
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetManaAttribute()).AddUObject(this, &ThisClass::ManaChangedCallback);

				bFoundAbility = true;
				break;
			}
		}
	}

	if (bFoundAbility)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULuosUserWidget_Ability::ManaChangedCallback(const FOnAttributeChangeData& Data)
{
	if (IsValid(AbilitySystemComponent.Get()))
	{
		ULuosGameplayAbility* ActivatableAbility = Cast<ULuosGameplayAbility>(CurrentAbilitySpec.Ability);
		if (ActivatableAbility->CheckCost(CurrentAbilitySpec.Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
		{
			AbilityBorder->SetIsEnabled(true);
		}
		else
		{
			AbilityBorder->SetIsEnabled(false);
		}
	}
}

void ULuosUserWidget_Ability::AbilityAddedCallback(FGameplayAbilitySpec& AddedAbilitySpec)
{
	if (AddedAbilitySpec.Ability && AddedAbilitySpec.DynamicAbilityTags.HasTag(AbilityInputTag))
	{
		InitializeAbilities();
	}
}

void ULuosUserWidget_Ability::AbilityRemovedCallback(FGameplayAbilitySpec& RemovedAbilitySpec)
{
	if (RemovedAbilitySpec.Ability && RemovedAbilitySpec.DynamicAbilityTags.HasTag(AbilityInputTag))
	{
		InitializeAbilities();
	}
}
