

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "LuosAIController.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ALuosAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALuosAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void SetMotionWarpingTarget(AActor* Target);

protected:
	UFUNCTION(BlueprintCallable, Category = "Luos|AI")
	void RunAI();

	UFUNCTION(BlueprintCallable, Category = "Luos|AI")
	void RestartAI();

	UFUNCTION(BlueprintCallable, Category = "Luos|AI")
	void StopAI();

	UFUNCTION()
	void TargetPerceptionUpdatedCallback(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void TargetPerceptionForgottenCallback(AActor* Actor); 

public:
	static FName HomeLocationKeyName;
	static FName MoveLocationKeyName;
	static FName TargetActorKeyName;
	static FName BehaviorStateKeyName;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Luos|Blackboard")
	class UBlackboardData* BlackboardData;

	UPROPERTY(EditDefaultsOnly, Category = "Luos|BehaviorTree")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UAISenseConfig_Sight* SightSenseConfig;
	UPROPERTY()
	class UAISenseConfig_Damage* DamageSenseConfig;
	UPROPERTY()
	class UAISenseConfig_Hearing* HearingSenseConfig;
};
