

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "LuosProjectile.generated.h"

UCLASS()
class LUOS_API ALuosProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ALuosProjectile();

	UFUNCTION(BlueprintPure, Category = "Luos|Projectile")
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, Category = "Luos|Projectile")
	void AddImpactGameplayEffectSpec(FGameplayEffectSpecHandle ImpactGameplayEffectSpec);

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void ProjectileBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Luos|Gameplay Cue", meta = (Categories = "GameplayCue"))
	FGameplayTag ImpactGameplayCueTag;

	UPROPERTY()
	TArray<FGameplayEffectSpecHandle> ImpactGameplayEffectSpecs;
};
