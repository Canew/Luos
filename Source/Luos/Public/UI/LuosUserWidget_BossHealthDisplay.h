

#pragma once

#include "CoreMinimal.h"
#include "UI/LuosUserWidget.h"
#include "GameplayEffectTypes.h"
#include "LuosUserWidget_BossHealthDisplay.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosUserWidget_BossHealthDisplay : public ULuosUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetBossActor(AActor* NewBossActor);

protected:
	virtual void HealthChangedCallback(const FOnAttributeChangeData& Data);

protected:
	const AActor* BossActor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BossNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> BossHealthDisplayBar;
};
