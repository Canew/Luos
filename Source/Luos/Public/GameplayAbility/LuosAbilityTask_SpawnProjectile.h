

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayActor/LuosProjectile.h"
#include "GameplayEffectTypes.h"
#include "LuosAbilityTask_SpawnProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLuosSpawnProjectileDelegate, ALuosProjectile*, SpawnedProjectile);

/**
 * 
 */
UCLASS()
class LUOS_API ULuosAbilityTask_SpawnProjectile : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FLuosSpawnProjectileDelegate Success;

	/** Called when we can't spawn: on clients or potentially on server if they fail to spawn (rare) */
	UPROPERTY(BlueprintAssignable)
	FLuosSpawnProjectileDelegate DidNotSpawn;

	/** Spawn new Actor on the network authority (server) */
	UFUNCTION(BlueprintCallable, Category = "Luos|Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", AutoCreateRefTerm = "ImpactEffectSpecs"))
	static ULuosAbilityTask_SpawnProjectile* SpawnProjectile(UGameplayAbility* OwningAbility, FVector SpawnLocation, FRotator SpawnRotation, FVector SpawnVelocity, bool bSetVelocityToCursor, TSubclassOf<ALuosProjectile> Class, const TArray<FGameplayEffectSpecHandle> ImpactEffectSpecs);

	UFUNCTION(BlueprintCallable, Category = "Luos|Abilities", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	virtual bool BeginSpawningActor(UGameplayAbility* OwningAbility, ALuosProjectile*& SpawnedActor);

	UFUNCTION(BlueprintCallable, Category = "Luos|Abilities", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	virtual void FinishSpawningActor(UGameplayAbility* OwningAbility, ALuosProjectile* SpawnedActor);

protected:
	float GetHeightCorrection() const;

protected:
	FVector SpawnLocation;
	FRotator SpawnRotation;
	FVector SpawnVelocity;
	bool bSetVelocityToCursor;
	TArray<FGameplayEffectSpecHandle> ImpactEffectSpecs;

	UPROPERTY()
	TSubclassOf<ALuosProjectile> ProjectileClassToSpawn;
};
