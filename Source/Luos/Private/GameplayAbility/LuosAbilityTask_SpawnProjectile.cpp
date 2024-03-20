


#include "GameplayAbility/LuosAbilityTask_SpawnProjectile.h"
#include "AbilitySystemComponent.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/LuosPlayerController.h"
#include "GameplayActor/LuosProjectile.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAbilityTask_SpawnProjectile)

ULuosAbilityTask_SpawnProjectile* ULuosAbilityTask_SpawnProjectile::SpawnProjectile(UGameplayAbility* OwningAbility, FVector SpawnLocation, FRotator SpawnRotation, FVector SpawnVelocity, bool bSetVelocityToCursor, TSubclassOf<ALuosProjectile> ProjectileClass, const TArray<FGameplayEffectSpecHandle> ImpactEffectSpecs)
{
	ULuosAbilityTask_SpawnProjectile* MyObj = NewAbilityTask<ULuosAbilityTask_SpawnProjectile>(OwningAbility);
	MyObj->SpawnLocation = SpawnLocation;
	MyObj->SpawnRotation = SpawnRotation;
	MyObj->SpawnVelocity = SpawnVelocity;
	MyObj->ProjectileClassToSpawn = ProjectileClass;
	MyObj->bSetVelocityToCursor = bSetVelocityToCursor;
	MyObj->ImpactEffectSpecs = ImpactEffectSpecs;

	return MyObj;
}

bool ULuosAbilityTask_SpawnProjectile::BeginSpawningActor(UGameplayAbility* OwningAbility, ALuosProjectile*& SpawnedActor)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(GetOwnerActor(), EGetWorldErrorMode::LogAndReturnNull);
	if (World)
	{
		SpawnedActor = World->SpawnActorDeferred<ALuosProjectile>(ProjectileClassToSpawn, FTransform(SpawnRotation, SpawnLocation), GetAvatarActor(), NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	}

	if (SpawnedActor == nullptr)
	{
		DidNotSpawn.Broadcast(nullptr);
		return false;
	}

	FVector NormalizedVelocity = GetAvatarActor()->GetActorForwardVector();
	if (bSetVelocityToCursor)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetAvatarActor()->GetOwner()))
		{
			FHitResult HitResult;
			if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
			{

				NormalizedVelocity = ((HitResult.Location + FVector(0.0f, 0.0f, GetHeightCorrection())) - GetAvatarActor()->GetActorLocation()).GetSafeNormal();
			}
		}
	}
	else
	{
		NormalizedVelocity = SpawnVelocity;
	}

	SpawnedActor->GetProjectileMovement()->Velocity = NormalizedVelocity;

	for (const FGameplayEffectSpecHandle& ImpactEffectSpec : ImpactEffectSpecs)
	{
		SpawnedActor->AddImpactGameplayEffectSpec(ImpactEffectSpec);
	}

	return true;
}

void ULuosAbilityTask_SpawnProjectile::FinishSpawningActor(UGameplayAbility* OwningAbility, ALuosProjectile* SpawnedActor)
{
	if (SpawnedActor)
	{
		const FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		SpawnedActor->FinishSpawning(SpawnTransform);
		Success.Broadcast(SpawnedActor);
	}

	EndTask();
}

float ULuosAbilityTask_SpawnProjectile::GetHeightCorrection() const
{
	if (UCapsuleComponent* CapsuleComponent = GetAvatarActor()->FindComponentByClass<UCapsuleComponent>())
	{
		return CapsuleComponent->GetScaledCapsuleHalfHeight();
	}

	return 0.0f;
}
