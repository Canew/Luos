

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "LuosCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual float GetMaxSpeed() const override;

	virtual void RegisterGameplayEvent();

	virtual void Strafe();
	virtual void UnStrafe();
	virtual bool IsStrafing() const;

	virtual void Sprint();
	virtual void UnSprint();
	virtual bool IsSprinting() const;

protected:
	UPROPERTY()
	bool bIsStrafed;

	UPROPERTY()
	bool bIsSprinted;

	UPROPERTY();
	bool bIsStunned;
};
