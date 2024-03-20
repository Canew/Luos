


#include "UI/LuosUserWidget_Status.h"
#include "Components/SizeBox.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility/LuosAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosUserWidget_Status)

void ULuosUserWidget_Status::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeStatus();
}

void ULuosUserWidget_Status::InitializeStatus()
{
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerPawn()))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::UpdateHealth);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::UpdateHealth);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetManaAttribute()).AddUObject(this, &ThisClass::UpdateMana);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::UpdateMana);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetStaminaAttribute()).AddUObject(this, &ThisClass::UpdateStamina);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &ThisClass::UpdateStamina);

		UpdateHealth();
		UpdateMana();
		UpdateStamina();
	}
	else
	{
		APlayerController* PlayerController = GetOwningPlayer();
		if (PlayerController)
		{
			PlayerController->GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::InitializeStatus);
		}
	}
}

void ULuosUserWidget_Status::UpdateHealth(const FOnAttributeChangeData& Data)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerPawn()))
	{
		FGameplayAttribute HealthAttribute = ULuosAttributeSet::GetHealthAttribute();
		FGameplayAttribute MaxHealthAttribute = ULuosAttributeSet::GetMaxHealthAttribute();
		if (AbilitySystemComponent->HasAttributeSetForAttribute(HealthAttribute) && AbilitySystemComponent->HasAttributeSetForAttribute(MaxHealthAttribute))
		{
			float HealthValue = AbilitySystemComponent->GetNumericAttribute(HealthAttribute);
			float MaxHealthValue = AbilitySystemComponent->GetNumericAttribute(MaxHealthAttribute);

			if (MaxHealthValue == 0.0f)
			{
				HealthProgressBar->SetPercent(0.0f);
			}
			else
			{
				HealthProgressBar->SetPercent(HealthValue / MaxHealthValue);
			}

			HealthSizeBox->SetWidthOverride(MaxHealthValue * 2.0f);
		}
	}
}

void ULuosUserWidget_Status::UpdateMana(const FOnAttributeChangeData& Data)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerPawn()))
	{
		FGameplayAttribute ManaAttribute = ULuosAttributeSet::GetManaAttribute();
		FGameplayAttribute MaxManaAttribute = ULuosAttributeSet::GetMaxManaAttribute();
		if (AbilitySystemComponent->HasAttributeSetForAttribute(ManaAttribute) && AbilitySystemComponent->HasAttributeSetForAttribute(MaxManaAttribute))
		{
			float ManaValue = AbilitySystemComponent->GetNumericAttribute(ManaAttribute);
			float MaxManaValue = AbilitySystemComponent->GetNumericAttribute(MaxManaAttribute);

			if (MaxManaValue == 0.0f)
			{
				ManaProgressBar->SetPercent(0.0f);
			}
			else
			{
				ManaProgressBar->SetPercent(ManaValue / MaxManaValue);
			}

			ManaSizeBox->SetWidthOverride(MaxManaValue * 2.0f);
		}
	}
}

void ULuosUserWidget_Status::UpdateStamina(const FOnAttributeChangeData& Data)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerPawn()))
	{
		FGameplayAttribute StaminaAttribute = ULuosAttributeSet::GetStaminaAttribute();
		FGameplayAttribute MaxStaminaAttribute = ULuosAttributeSet::GetMaxStaminaAttribute();
		if (AbilitySystemComponent->HasAttributeSetForAttribute(StaminaAttribute) && AbilitySystemComponent->HasAttributeSetForAttribute(MaxStaminaAttribute))
		{
			float StaminaValue = AbilitySystemComponent->GetNumericAttribute(StaminaAttribute);
			float MaxStaminaValue = AbilitySystemComponent->GetNumericAttribute(MaxStaminaAttribute);

			if (MaxStaminaValue == 0.0f)
			{
				StaminaProgressBar->SetPercent(0.0f);
			}
			else
			{
				StaminaProgressBar->SetPercent(StaminaValue / MaxStaminaValue);
			}

			StaminaSizeBox->SetWidthOverride(MaxStaminaValue * 2.0f);
		}
	}
}

