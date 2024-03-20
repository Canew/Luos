

#pragma once

#include "CoreMinimal.h"
#include "UI/LuosUserWidget.h"
#include "LuosUserWidget_Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosUserWidget_Crosshair : public ULuosUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> CrosshairImage;
};
