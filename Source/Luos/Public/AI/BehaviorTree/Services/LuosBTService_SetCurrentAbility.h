

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "LuosBTService_SetCurrentAbility.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosBTService_SetCurrentAbility : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	ULuosBTService_SetCurrentAbility();

protected:
	/** called when auxiliary node becomes active
 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	/** update next tick interval
	  * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
