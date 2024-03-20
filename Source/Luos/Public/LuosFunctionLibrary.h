

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "LuosFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Luos|Collision", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Cone Overlap Actors"))
	static bool ConeOverlapActors(const UObject* WorldContextObject, const FVector ConePos, float ConeRadius, const FVector ConeDirection, float ConeHalfAngleDegree, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category = "Luos|Collision", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Cone Overlap Components"))
	static bool ConeOverlapComponents(const UObject* WorldContextObject, const FVector ConePos, float ConeRadius, const FVector ConeDirection, float ConeHalfAngleDegree, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class UPrimitiveComponent*>& OutComponents);

	UFUNCTION(BlueprintPure, Category = "Luos|Ability")
	static float GetSetByCallerMagnitude(const FGameplayEffectSpecHandle& EffectSpecHandle, FGameplayTag DataTag, bool WarnIfNotFound = false, float DefaultIfNotFound = 0.f);

	UFUNCTION(BlueprintPure, Category = "Luos|Ability")
	static bool CanApplyGameplayEffect(const FGameplayEffectSpecHandle& EffectSpecHandle, const FGameplayAbilityTargetDataHandle& TargetData);
};
