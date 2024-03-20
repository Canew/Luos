

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "LuosAnimNotifyState_GameplayEvent.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Gameplay Event"))
class LUOS_API ULuosAnimNotifyState_GameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay Tag")
	FGameplayTagContainer BeginGameplayTags;

	UPROPERTY(EditAnywhere, Category = "Gameplay Tag")
	FGameplayTagContainer TickGameplayTags;

	UPROPERTY(EditAnywhere, Category = "Gameplay Tag")
	FGameplayTagContainer EndGameplayTags;
};
