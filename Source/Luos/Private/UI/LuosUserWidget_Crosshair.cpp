


#include "UI/LuosUserWidget_Crosshair.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SGameLayerManager.h"

void ULuosUserWidget_Crosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UWorld* World = GetOwningPlayer()->GetWorld();
	if (!World || !World->IsGameWorld())
	{
		return;
	}

	FVector2D PlayerMousePositionOnViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	CrosshairImage->SetRenderTranslation(PlayerMousePositionOnViewport);

	if (UGameViewportClient* ViewportClient = World->GetGameViewport())
	{
		FVector2D PlayerMousePosition;
		GetOwningPlayer()->GetMousePosition(PlayerMousePosition.X, PlayerMousePosition.Y);

		FVector2D ViewportSize;
		ViewportClient->GetViewportSize(ViewportSize);

		FVector2D ViewportCenter = ViewportSize / 2.0f;
		float Angle = 180.0f / UE_DOUBLE_PI * FMath::Acos(FVector2D::DotProduct((FVector2D(ViewportCenter.X, 0.0f) - ViewportCenter).GetSafeNormal(), (PlayerMousePosition - ViewportCenter).GetSafeNormal()));
		if (PlayerMousePosition.X < ViewportCenter.X)
		{
			Angle *= -1.0f;
		}

		CrosshairImage->SetRenderTransformAngle(Angle);
	}
}
