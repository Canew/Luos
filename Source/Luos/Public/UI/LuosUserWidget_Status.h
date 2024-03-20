

#pragma once

#include "CoreMinimal.h"
#include "UI/LuosUserWidget.h"
#include "GameplayEffectTypes.h"
#include "LuosUserWidget_Status.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosUserWidget_Status : public ULuosUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void InitializeStatus();

	virtual void UpdateHealth(const FOnAttributeChangeData& Data = FOnAttributeChangeData());
	virtual void UpdateMana(const FOnAttributeChangeData& Data = FOnAttributeChangeData());
	virtual void UpdateStamina(const FOnAttributeChangeData& Data = FOnAttributeChangeData());

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class USizeBox> HealthSizeBox;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgressBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class USizeBox> ManaSizeBox;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UProgressBar> ManaProgressBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class USizeBox> StaminaSizeBox;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UProgressBar> StaminaProgressBar;
};
