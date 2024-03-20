

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ObjectMacros.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "LuosAbilityTask_WaitAbilityEnd.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLuosWaitAbilityEndDelegate, UGameplayAbility*, EndedAbility);

/**
 * 
 */
UCLASS()
class LUOS_API ULuosAbilityTask_WaitAbilityEnd : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Activate() override;

	UFUNCTION()
	void HandleAbilityEnded(UGameplayAbility* EndedAbility);

	/** Wait until a ability (of the same or different type) is ended. Only input based abilities will be counted unless IncludeTriggeredAbilities is true. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULuosAbilityTask_WaitAbilityEnd* WaitForAbilityEnd(UGameplayAbility* OwningAbility, FGameplayTag WithTag, FGameplayTag WithoutTag, bool TriggerOnce = true);

	/** Wait until a ability (of the same or different type) is ended. Only input based abilities will be counted unless IncludeTriggeredAbilities is true. Uses a tag requirements structure to filter abilities. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULuosAbilityTask_WaitAbilityEnd* WaitForAbilityEndWithTagRequirements(UGameplayAbility* OwningAbility, FGameplayTagRequirements TagRequirements, bool TriggerOnce = true);

	/** Wait until a ability (of the same or different type) is ended. Only input based abilities will be counted unless IncludeTriggeredAbilities is true. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULuosAbilityTask_WaitAbilityEnd* WaitForAbilityEnd_Query(UGameplayAbility* OwningAbility, FGameplayTagQuery Query, bool TriggerOnce = true);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULuosAbilityTask_WaitAbilityEnd* WaitForAbilityEndWithClass(UGameplayAbility* OwningAbility, TSubclassOf<UGameplayAbility> AbilityClass, bool TriggerOnce = true);

protected:
	virtual void OnDestroy(bool AbilityEnded) override;

public:
	UPROPERTY(BlueprintAssignable)
	FLuosWaitAbilityEndDelegate OnAbilityEnd;

protected:
	FGameplayTag WithTag;
	FGameplayTag WithoutTag;
	bool TriggerOnce;
	FGameplayTagRequirements TagRequirements;

	FGameplayTagQuery Query;

	TSubclassOf<UGameplayAbility> AbilityClass;

	FDelegateHandle OnAbilityEndDelegateHandle;
};
