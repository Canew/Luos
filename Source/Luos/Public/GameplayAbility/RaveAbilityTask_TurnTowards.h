

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RaveAbilityTask_TurnTowards.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRaveTurnTowardsCompleteDelegate);

/**
 * 
 */
UCLASS()
class LUOS_API URaveAbilityTask_TurnTowards : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	URaveAbilityTask_TurnTowards();
	
	/** Turn towards specified direction. direction is world space vector */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static URaveAbilityTask_TurnTowards* TurnTowards(UGameplayAbility* OwningAbility, FVector Direction, float CompleteThreshold, bool bTurnInstantly, bool bEndTaskAfterTurn);

	virtual void Activate() override;

	/** Tick function for this task, if bTickingTask == true */
	virtual void TickTask(float DeltaTime) override;

public:
	UPROPERTY(BlueprintAssignable)
	FRaveTurnTowardsCompleteDelegate OnCompleted;

private:
	TWeakObjectPtr<class ALuosCharacter> AvatarCharacter;

	FVector Direction;
	float CompleteThreshold;
	bool bTurnInstantly;
	bool bEndTaskAfterTurn;

};
