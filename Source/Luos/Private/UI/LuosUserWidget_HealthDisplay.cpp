


#include "UI/LuosUserWidget_HealthDisplay.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbility/LuosAttributeSet.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "Character/LuosCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosUserWidget_HealthDisplay)

void ULuosUserWidget_HealthDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	USizeBox* HealthDisplaySizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("HealthDisplaySizeBox"));
	if (HealthDisplaySizeBox)
	{
		WidgetTree->RootWidget = HealthDisplaySizeBox;
		HealthDisplaySizeBox->SetWidthOverride(100.0f);
		HealthDisplaySizeBox->SetHeightOverride(5.0f);

		HealthDisplayBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("HealthDisplayBar"));
		if (HealthDisplayBar)
		{
			HealthDisplayBar->SetFillColorAndOpacity(FLinearColor(0.5f, 0.01f, 0.01f, 0.9f));

			FSlateBrush ProgressBarStyleBrush;
			ProgressBarStyleBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

			FSlateBrushOutlineSettings ProgressBarBrushOutlineSettings;
			ProgressBarBrushOutlineSettings.RoundingType = ESlateBrushRoundingType::HalfHeightRadius;
			ProgressBarStyleBrush.OutlineSettings = ProgressBarBrushOutlineSettings;

			FProgressBarStyle ProgressBarStyle;
			ProgressBarStyle.SetBackgroundImage(ProgressBarStyleBrush);
			ProgressBarStyle.SetFillImage(ProgressBarStyleBrush);
			HealthDisplayBar->SetWidgetStyle(ProgressBarStyle);

			USizeBoxSlot* SizeBoxSlot = Cast<USizeBoxSlot>(HealthDisplaySizeBox->AddChild(HealthDisplayBar));
			SizeBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			SizeBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}
	}
}

void ULuosUserWidget_HealthDisplay::SetOwningActor(AActor* NewOwningActor)
{
	if (!IsValid(NewOwningActor))
	{
		return;
	}

	OwningActor = NewOwningActor;

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);
	if (AbilitySystemComponent)
	{
		bool bHealthAttributeFound = false;
		bool bMaxHealthAttributeFound = false;
		float CurrentHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetHealthAttribute(), bHealthAttributeFound);
		float MaxHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetMaxHealthAttribute(), bMaxHealthAttributeFound);

		if (bHealthAttributeFound && bMaxHealthAttributeFound && MaxHealth != 0.0f)
		{
			HealthDisplayBar->SetPercent(CurrentHealth / MaxHealth);
		}

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChangedCallback);
	}
}

void ULuosUserWidget_HealthDisplay::HealthChangedCallback(const FOnAttributeChangeData& Data)
{
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);
	if (AbilitySystemComponent)
	{
		bool bHealthAttributeFound = false;
		bool bMaxHealthAttributeFound = false;
		float CurrentHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetHealthAttribute(), bHealthAttributeFound);
		float MaxHealth = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetMaxHealthAttribute(), bMaxHealthAttributeFound);

		if (bHealthAttributeFound && bMaxHealthAttributeFound && MaxHealth != 0.0f)
		{
			HealthDisplayBar->SetPercent(CurrentHealth / MaxHealth);
		}
	}
}
