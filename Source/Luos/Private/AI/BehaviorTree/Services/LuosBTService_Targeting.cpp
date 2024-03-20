


#include "AI/BehaviorTree/Services/LuosBTService_Targeting.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/LuosAIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosBTService_Targeting)

ULuosBTService_Targeting::ULuosBTService_Targeting()
{
	NodeName = TEXT("Targeting");
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;

	BlackboardKey.AddObjectFilter(this, GetSelectedBlackboardKey(), AActor::StaticClass());
}

void ULuosBTService_Targeting::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ALuosAIController* AIController = Cast<ALuosAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return;
	}

	UAIPerceptionComponent* PerceptionComponent = AIController->GetPerceptionComponent();
	if (PerceptionComponent)
	{
		TArray<AActor*> PerceivedHostileActors;
		PerceptionComponent->GetPerceivedHostileActors(PerceivedHostileActors);

		if (PerceivedHostileActors.Num() > 0)
		{
			int32 PerceivedHostileActorIndex = FMath::RandRange(0, PerceivedHostileActors.Num() - 1);

			AIController->SetFocus(PerceivedHostileActors[PerceivedHostileActorIndex]);
			AIController->SetMotionWarpingTarget(PerceivedHostileActors[PerceivedHostileActorIndex]);
			UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
			if (BlackboardComponent)
			{
				BlackboardComponent->SetValueAsObject(ALuosAIController::TargetActorKeyName, PerceivedHostileActors[PerceivedHostileActorIndex]);
			}
		}
		else
		{
			AIController->SetFocus(nullptr);
			AIController->SetMotionWarpingTarget(nullptr);
			UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
			if (BlackboardComponent)
			{
				BlackboardComponent->SetValueAsObject(ALuosAIController::TargetActorKeyName, nullptr);
			}
		}
	}
}
