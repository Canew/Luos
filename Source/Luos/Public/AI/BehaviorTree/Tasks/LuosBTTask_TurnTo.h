

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "LuosBTTask_TurnTo.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosBTTask_TurnTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	ULuosBTTask_TurnTo();

	/** @return name of node */
	FString GetNodeName_Implementation() const;

	/** starts this task, should return Succeeded, Failed or InProgress
 *  (use FinishLatentTask() when returning InProgress)
 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseCharacterRotationRate == false"))
	float TurnInterpSpeed;

	UPROPERTY(EditAnywhere)
	bool bUseCharacterRotationRate;
};
