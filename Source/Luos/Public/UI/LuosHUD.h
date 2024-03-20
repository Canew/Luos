

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LuosHUD.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ALuosHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void CreateBossHealthDisplayActor(AActor* NewBossActor);
	void RemoveBossHealthDisplayActor();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	TObjectPtr<UUserWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> CrosshairWidgetClass;
	TObjectPtr<UUserWidget> CrosshairWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class ULuosUserWidget_BossHealthDisplay> BossHealthDisplayWidgetClass;
	TObjectPtr<class ULuosUserWidget_BossHealthDisplay> BossHealthDisplayWidget;

	FTimerHandle BossHealthDisplayTimerHandle;
};
