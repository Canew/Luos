

#pragma once

#include "CoreMinimal.h"
#include "UI/LuosUserWidget.h"
#include "LuosUserWidget_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosUserWidget_Lobby : public ULuosUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	void StartButtonClickedCallback();
	void QuitButtonClickedCallback();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> QuitButton;
};
