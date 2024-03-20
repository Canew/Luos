

#pragma once

#include "CoreMinimal.h"
#include "UI/LuosUserWidget.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpec.h"
#include "LuosUserWidget_Ability.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosUserWidget_Ability : public ULuosUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void InitializeAbilitySystemComponent();
	virtual void InitializeAbilities();

	void ManaChangedCallback(const FOnAttributeChangeData& Data);
	void AbilityAddedCallback(FGameplayAbilitySpec& AddedAbilitySpec);
	void AbilityRemovedCallback(FGameplayAbilitySpec& RemovedAbilitySpec);
	
protected:
	TWeakObjectPtr<class ULuosAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> AbilityBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AbilityImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AbilityCostTextBlock;

	UPROPERTY(EditInstanceOnly, Category = "Luos|Ability")
	FGameplayTag AbilityInputTag;

	FGameplayAbilitySpec CurrentAbilitySpec;

	FTimerHandle RequestAbilitySystemComponentTimerHandle;
};
