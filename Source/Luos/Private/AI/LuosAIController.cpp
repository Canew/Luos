


#include "AI/LuosAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/LuosPlayerController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MotionWarpingComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAIController)

#define BOT_TEAM 2

FName ALuosAIController::HomeLocationKeyName = FName(TEXT("HomeLocation"));
FName ALuosAIController::MoveLocationKeyName = FName(TEXT("MoveLocation"));
FName ALuosAIController::TargetActorKeyName = FName(TEXT("TargetActor"));

ALuosAIController::ALuosAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	bAttachToPawn = true;
	bWantsPlayerState = true;

	// Setup the perception component
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception")));

	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSenseConfig"));
	SightSenseConfig->SightRadius = 2000.0f;
	SightSenseConfig->LoseSightRadius = 2000.0f;
	SightSenseConfig->SetMaxAge(8.0f);
	SightSenseConfig->PeripheralVisionAngleDegrees = 180.0f;
	GetPerceptionComponent()->ConfigureSense(*SightSenseConfig);

	DamageSenseConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSenseConfig"));
	DamageSenseConfig->SetMaxAge(8.0f);
	GetPerceptionComponent()->ConfigureSense(*DamageSenseConfig);

	HearingSenseConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingSenseConfig"));
	HearingSenseConfig->HearingRange = 350.0f;
	HearingSenseConfig->SetMaxAge(8.0f);
	GetPerceptionComponent()->ConfigureSense(*HearingSenseConfig);

	GetPerceptionComponent()->SetDominantSense(SightSenseConfig->GetSenseImplementation());
}

void ALuosAIController::BeginPlay()
{
	Super::BeginPlay();

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::TargetPerceptionUpdatedCallback);
	GetPerceptionComponent()->OnTargetPerceptionForgotten.AddDynamic(this, &ThisClass::TargetPerceptionForgottenCallback);
	UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*GetAIPerceptionComponent());

	RunAI();
	if (APawn* MyPawn = GetPawn())
	{
		if (Blackboard && Blackboard->IsValidKey(Blackboard->GetKeyID(HomeLocationKeyName)))
		{
			Blackboard->SetValueAsVector(HomeLocationKeyName, MyPawn->GetActorLocation());
		}
	}

	// Generic team
	SetGenericTeamId(BOT_TEAM);
}

void ALuosAIController::SetMotionWarpingTarget(AActor* Target)
{
	if (!GetPawn())
	{
		return;
	}

	if (UMotionWarpingComponent* MotionWarpingComponent = GetPawn()->FindComponentByClass<UMotionWarpingComponent>())
	{
		if (IsValid(Target))
		{
			MotionWarpingComponent->AddOrUpdateWarpTarget(FMotionWarpingTarget(TEXT("Target"), Target->GetRootComponent(), NAME_None, true));
		}
		else
		{
			MotionWarpingComponent->RemoveWarpTarget(TEXT("Target"));
		}
	}
}

void ALuosAIController::RunAI()
{
	if (BlackboardData && BehaviorTree)
	{
		UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
		if (UseBlackboard(BlackboardData, BlackboardComponent))
		{
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void ALuosAIController::RestartAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}
}

void ALuosAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void ALuosAIController::TargetPerceptionUpdatedCallback(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		APawn* UpdatedPawn = Cast<APawn>(Actor);
		if (IsValid(UpdatedPawn) && GetPawn() && GetPawn()->ActorHasTag(TEXT("Boss")))
		{
			ALuosPlayerController* PlayerController = UpdatedPawn->GetController<ALuosPlayerController>();
			if (IsValid(PlayerController))
			{
				PlayerController->CreateBossHealthDisplayActor(GetPawn());
			}
		}
	}
}	

void ALuosAIController::TargetPerceptionForgottenCallback(AActor* Actor)
{
	APawn* ForgottenPawn = Cast<APawn>(Actor);
	if (IsValid(ForgottenPawn) && GetPawn() && GetPawn()->ActorHasTag(TEXT("Boss")))
	{
		ALuosPlayerController* PlayerController = ForgottenPawn->GetController<ALuosPlayerController>();
		if (IsValid(PlayerController))
		{
			PlayerController->RemoveBossHealthDisplayActor();
		}
	}
}
