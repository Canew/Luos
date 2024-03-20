

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "LuosAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PushDisableRootMotion(UAnimMontage* Montage, float Duration = 0.0f);
	void PopDisableRootMotion(UAnimMontage* Montage);

private:
	void InitializeProperties();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Reference")
	TWeakObjectPtr<class ACharacter> Character;

	UPROPERTY(BlueprintReadWrite, Category = "Reference")
	TWeakObjectPtr<class ULuosCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Reference")
	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool ShouldMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsStrafing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsSprinting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsFalling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsStunned;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveDirection;

	/** if character should play full body animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	bool bFullBody;

private:
	FTimerHandle InitializeTimerHandle;
};
