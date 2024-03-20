


#include "Animation/LuosAnimNotify_GameplayEvent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAnimNotify_GameplayEvent)

FString ULuosAnimNotify_GameplayEvent::GetNotifyName_Implementation() const
{
	return TEXT("Gameplay Event");
}

void ULuosAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()))
	{
		for (FGameplayTag GameplayTag : GameplayTags)
		{
			FGameplayEventData GameplayEventData;
			GameplayEventData.ContextHandle = ASC->MakeEffectContext();
			GameplayEventData.OptionalObject = Animation;
			ASC->HandleGameplayEvent(GameplayTag, &GameplayEventData);
		}
	}
}
