

#pragma once

#include "CoreMinimal.h"
#include "UI/LuosUserWidget.h"
#include "GameplayEffectTypes.h"
#include "LuosUserWidget_HealthDisplay.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosUserWidget_HealthDisplay : public ULuosUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetOwningActor(AActor* NewOwningActor);

protected:
	virtual void HealthChangedCallback(const FOnAttributeChangeData& Data);

protected:
	const AActor* OwningActor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthDisplayBar;
};
