

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LuosGameplayAbility.generated.h"

/**
 * ELuosAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class ELuosAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn,

	// Try to activate only when event is received.
	OnEventReceived
};


/**
 * ELuosAbilityActivationGroup
 *
 *	Defines how an ability activates in relation to other abilities.
 */
UENUM(BlueprintType)
enum class ELuosAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.
	Exclusive_Replaceable,

	// Ability blocks all other exclusive abilities from activating.
	Exclusive_Blocking,
};

USTRUCT(BlueprintType)
struct FLuosAbilityDescription
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (MultiLine = "true"))
	FText Description;
};

UENUM(BlueprintType)
enum class FLuosGrantedEffectActivationPolicy : uint8
{
	Always,
	OnActivated,
	NotOnActivated,
};

USTRUCT(BlueprintType)
struct FLuosGrantedGameplayEffectDef
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat EffectLevel;

	UPROPERTY(EditDefaultsOnly)
	FLuosGrantedEffectActivationPolicy EffectActivationPolicy;

	UPROPERTY(EditDefaultsOnly)
	bool bClearOnAbilityRemoved = true;


	UPROPERTY()
	FActiveGameplayEffectHandle ActiveEffectHandle;
};

/**
 * 
 */
UCLASS()
class LUOS_API ULuosGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULuosGameplayAbility();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	ELuosAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	ELuosAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }
	FLuosAbilityDescription GetAbilityDescription() const { return AbilityDescription; }
	TArray<FLuosGrantedGameplayEffectDef> GetGrantedEffects() const { return GrantedEffects; }
	float GetCooldownDuration() const { return CooldownTime; };

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	UFUNCTION(BlueprintPure, Category = "Luos|Ability")
	virtual class ULuosAbilitySystemComponent* GetLuosAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Luos|Ability")
	virtual int32 GetPredictionKey() const;

	UFUNCTION(BlueprintPure, Category = "Luos|Ability")
	FGameplayEffectSpecHandle MakeGameplayEffectSpecFromHitResult(TSubclassOf<UGameplayEffect> GameplayEffectClass, float GameplayEffectLevel = 1.0f, const FHitResult& HitResult = FHitResult());

private:
	void CancellableGameplayEventCallback(const FGameplayEventData* Payload);

protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Luos|Ability Activation")
	ELuosAbilityActivationPolicy ActivationPolicy;

	// Defines the relationship between this ability activating and other abilities activating.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Luos|Ability Activation")
	ELuosAbilityActivationGroup ActivationGroup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Luos|Description")
	FLuosAbilityDescription AbilityDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cooldowns")
	float CooldownTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Granted Effects")
	TArray<FLuosGrantedGameplayEffectDef> GrantedEffects;

	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer CooldownTagsContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Events")
	bool bWaitCancellableEvent;

private:
	FDelegateHandle CancellableEventMulticastDelegateHandle;
};
