


#include "UI/LuosHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "UI/LuosUserWidget_BossHealthDisplay.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosHUD)

void ALuosHUD::BeginPlay()
{
	Super::BeginPlay();

	GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
	GetOwningPlayerController()->SetShowMouseCursor(false);

	HUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetClass, TEXT("HUD"));
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

	CrosshairWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), CrosshairWidgetClass, TEXT("Crosshair"));
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
}

void ALuosHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
	}

	if (CrosshairWidget)
	{
		CrosshairWidget->RemoveFromParent();
		CrosshairWidget = nullptr;
	}

	GetWorldTimerManager().ClearTimer(BossHealthDisplayTimerHandle);
}

void ALuosHUD::CreateBossHealthDisplayActor(AActor* NewBossActor)
{
	if (!IsValid(NewBossActor) || BossHealthDisplayWidget)
	{
		return;
	}

	BossHealthDisplayWidget = CreateWidget<ULuosUserWidget_BossHealthDisplay>(GetOwningPlayerController(), BossHealthDisplayWidgetClass, TEXT("BossHealthDisplay"));
	if (BossHealthDisplayWidget)
	{
		BossHealthDisplayWidget->SetBossActor(NewBossActor);
		BossHealthDisplayWidget->AddToViewport();

		GetWorldTimerManager().SetTimer(BossHealthDisplayTimerHandle, FTimerDelegate::CreateLambda(
			[this, NewBossActor]() -> void
			{
				if (!IsValid(NewBossActor))
				{
					RemoveBossHealthDisplayActor();
				}
			}), 1.0f, true);
	}
}

void ALuosHUD::RemoveBossHealthDisplayActor()
{
	if (BossHealthDisplayWidget)
	{
		BossHealthDisplayWidget->RemoveFromParent();
		BossHealthDisplayWidget = nullptr;
	}
}
