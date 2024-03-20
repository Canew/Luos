

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "LuosExecutionCalculation_Force.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosExecutionCalculation_Force : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	UPROPERTY()
	TSubclassOf<UGameplayEffect> StunEffectClass;
};
