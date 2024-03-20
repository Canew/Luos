


#include "AI/BehaviorTree/Tasks/LuosBTTask_TryActivateAbility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosBTTask_TryActivateAbility)

ULuosBTTask_TryActivateAbility::ULuosBTTask_TryActivateAbility()
{
	NodeName = FString(TEXT("Try Activate Ability"));
	bNotifyTick = true;
	bCreateNodeInstance = true;

	TSubclassOf<UGameplayAbility> SubclassOfGameplayAbility = UGameplayAbility::StaticClass();
	BlackboardKey.AddClassFilter(this, GetSelectedBlackboardKey(), SubclassOfGameplayAbility);
}

EBTNodeResult::Type ULuosBTTask_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerBehaviorComponent = &OwnerComp;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AbilitySystemInterface)
	{
		return EBTNodeResult::Failed;
	}

	AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return EBTNodeResult::Failed;
	}

	CurrentAbilityClass = AIController->GetBlackboardComponent()->GetValueAsClass(GetSelectedBlackboardKey());
	CurrentAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(CurrentAbilityClass);
	CurrentAbilitySpec->SourceObject = AIController->GetPawn();
	if (CurrentAbilitySpec)
	{
		if (AbilitySystemComponent->TryActivateAbility(CurrentAbilitySpec->Handle))
		{
			if (!AbilitySystemComponent->AbilityEndedCallbacks.IsBoundToObject(this))
			{
				OnAbilityEndedDelegateHandle = AbilitySystemComponent->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnAbilityEnded);
			}

			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void ULuosBTTask_TryActivateAbility::FinishLatentTask(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult) const
{
	Super::FinishLatentTask(OwnerComp, TaskResult);

	if (OnAbilityEndedDelegateHandle.IsValid())
	{
		AbilitySystemComponent->AbilityEndedCallbacks.Remove(OnAbilityEndedDelegateHandle);
	}
}

void ULuosBTTask_TryActivateAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (FGameplayAbilitySpec* GameplayAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(CurrentAbilitySpec->Handle))
	{
		if (!GameplayAbilitySpec->IsActive())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}

void ULuosBTTask_TryActivateAbility::OnAbilityEnded(UGameplayAbility* Ability)
{
	if (!Ability || !CurrentAbilitySpec)
	{
		return;
	}

	if (Ability->GetClass() == CurrentAbilityClass)
	{
		FinishLatentTask(*OwnerBehaviorComponent, EBTNodeResult::Succeeded);
		return;
	}
}
