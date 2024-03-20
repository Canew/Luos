

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/LuosGameplayAbility.h"
#include "LuosGameplayAbility_Chain.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosGameplayAbility_Chain : public ULuosGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULuosGameplayAbility_Chain();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	float GetMaxChainInterval() const;
	float GetRemainingChainDuration() const;

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	bool CanActivateChainAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	bool ActivateChainAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION()
	void HandleInputPress(float TimeWaited);
	UFUNCTION()
	void HandleAbilityEnded(UGameplayAbility* Ability);

	void WaitInputPress();
	void HandleChainTimerElapsed();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Luos|Ability")
	TArray<TSubclassOf<ULuosGameplayAbility>> ChainAbilities;

	// MaxChainInterval == 0.0f, instantly end ability when end sub ability.
	// MaxChainInterval < 0.0f, has infinite chain interval.
	UPROPERTY(EditDefaultsOnly, Category = "Luos|Ability")
	float MaxChainInterval;

	FTimerHandle ChainTimerHandle;
	bool bCanActivateChainAbility;

	TObjectPtr<class UAbilityTask_WaitInputPress> WaitInputPressTask;
	TObjectPtr<class ULuosAbilityTask_WaitAbilityEnd> WaitAbilityEndTask;

	int32 CurrentChainAbilityIndex;
};
