


#include "Animation/LuosAnimNotifyState_GameplayEvent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAnimNotifyState_GameplayEvent)

FString ULuosAnimNotifyState_GameplayEvent::GetNotifyName_Implementation() const
{
	return TEXT("Gameplay Event");
}

void ULuosAnimNotifyState_GameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()))
	{
		for (FGameplayTag BeginGameplayTag : BeginGameplayTags)
		{
			FGameplayEventData GameplayEventData;
			GameplayEventData.ContextHandle = AbilitySystemComponent->MakeEffectContext();
			GameplayEventData.OptionalObject = Animation;
			AbilitySystemComponent->HandleGameplayEvent(BeginGameplayTag, &GameplayEventData);
		}
	}
}

void ULuosAnimNotifyState_GameplayEvent::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()))
	{
		for (FGameplayTag TickGameplayTag : TickGameplayTags)
		{
			FGameplayEventData GameplayEventData;
			GameplayEventData.ContextHandle = AbilitySystemComponent->MakeEffectContext();
			GameplayEventData.OptionalObject = Animation;
			AbilitySystemComponent->HandleGameplayEvent(TickGameplayTag, &GameplayEventData);
		}
	}
}

void ULuosAnimNotifyState_GameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()))
		for (FGameplayTag EndGameplayTag : EndGameplayTags)
		{
			FGameplayEventData GameplayEventData;
			GameplayEventData.ContextHandle = AbilitySystemComponent->MakeEffectContext();
			GameplayEventData.OptionalObject = Animation;
			AbilitySystemComponent->HandleGameplayEvent(EndGameplayTag, &GameplayEventData);
		}
}
