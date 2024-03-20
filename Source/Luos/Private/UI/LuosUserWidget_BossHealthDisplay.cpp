


#include "UI/LuosUserWidget_BossHealthDisplay.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbility/LuosAttributeSet.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "Character/LuosCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosUserWidget_BossHealthDisplay)

void ULuosUserWidget_BossHealthDisplay::SetBossActor(AActor* NewBossActor)
{
	if (!IsValid(NewBossActor))
	{
		return;
	}

	BossActor = NewBossActor;

	BossNameTextBlock->SetText(FText::FromString(BossActor->GetHumanReadableName()));

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(BossActor);
	if (AbilitySystemComponent)
	{
		bool bHealthAttributeFound = false;
		bool bMaxHealthAttributeFound = false;
		float CurrentHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetHealthAttribute(), bHealthAttributeFound);
		float MaxHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetMaxHealthAttribute(), bMaxHealthAttributeFound);

		if (bHealthAttributeFound && bMaxHealthAttributeFound && MaxHealth != 0.0f)
		{
			BossHealthDisplayBar->SetPercent(CurrentHealth / MaxHealth);
		}

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChangedCallback);
	}
}

void ULuosUserWidget_BossHealthDisplay::HealthChangedCallback(const FOnAttributeChangeData& Data)
{
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(BossActor);
	if (AbilitySystemComponent)
	{
		bool bHealthAttributeFound = false;
		bool bMaxHealthAttributeFound = false;
		float CurrentHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetHealthAttribute(), bHealthAttributeFound);
		float MaxHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetMaxHealthAttribute(), bMaxHealthAttributeFound);

		if (bHealthAttributeFound && bMaxHealthAttributeFound && MaxHealth != 0.0f)
		{
			BossHealthDisplayBar->SetPercent(CurrentHealth / MaxHealth);
		}
	}
}
