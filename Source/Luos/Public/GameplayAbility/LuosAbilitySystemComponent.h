

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility/LuosGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "NativeGameplayTags.h"
#include "LuosAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FLuosAbilityAdded, FGameplayAbilitySpec&)
DECLARE_MULTICAST_DELEGATE_OneParam(FLuosAbilityRemoved, FGameplayAbilitySpec&)

/**
 * 
 */
UCLASS()
class LUOS_API ULuosAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	ULuosAbilitySystemComponent();

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	bool CanApplyGameplayEffect(const FGameplayEffectSpecHandle& EffectSpecHandle) const;

	UFUNCTION(BlueprintCallable)
	void CancelAbilityClass(TSubclassOf<UGameplayAbility> InAbilityToCancel);

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void AbilityInstanceInputPressed(FGameplayAbilitySpec& Spec);
	virtual void AbilityInstanceInputReleased(FGameplayAbilitySpec& Spec);

public:
	FLuosAbilityAdded OnAbilityAdded;
	FLuosAbilityRemoved OnAbilityRemoved;

protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

#pragma region Action Buffer
public:
	UFUNCTION(BlueprintCallable, Category = "Luos|Input")
	virtual void SetRecentInputAction(const FGameplayAbilitySpecHandle& NewRecentInputAbilitySpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Luos|Input")
	virtual void InjectRecentInputForAction();

	virtual void ClearRecentInputAction();

protected:
	FGameplayAbilitySpecHandle RecentInputAbilitySpecHandle;

	FTimerHandle ActionBufferTimerHandle;
	float ActionBufferDuration;

#pragma endregion
};
