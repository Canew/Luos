

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "LuosAnimNotify_GameplayEvent.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Gameplay Event"))
class LUOS_API ULuosAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay Tag")
	FGameplayTagContainer GameplayTags;
};
