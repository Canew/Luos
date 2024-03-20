


#include "Character/LuosCharacterMovementComponent.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayAbility/LuosGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosCharacterMovementComponent)

float ULuosCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (bIsStunned)
	{
		return 0.0f;
	}

	if (IsStrafing())
	{
		MaxSpeed *= 0.4f;
	}
	else if (IsSprinting())
	{
		MaxSpeed *= 1.3f;
	}

	return MaxSpeed;
}

void ULuosCharacterMovementComponent::RegisterGameplayEvent()
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner());
	if (AbilitySystemInterface)
	{
		UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			// Strafe tag event
			AbilitySystemComponent->RegisterGameplayTagEvent(LuosGameplayTags::Character_Strafe).AddLambda(
				[this](const FGameplayTag EventTag, int32 Count) -> void
				{
					if (Count > 0)
					{
						Strafe();
					}
					else
					{
						UnStrafe();
					}
				});

			// Sprint tag event
			AbilitySystemComponent->RegisterGameplayTagEvent(LuosGameplayTags::Character_Sprint).AddLambda(
				[this](const FGameplayTag EventTag, int32 Count) -> void
				{
					if (Count > 0)
					{
						Sprint();
					}
					else
					{
						UnSprint();
					}
				});

			// Stun tag event
			AbilitySystemComponent->RegisterGameplayTagEvent(LuosGameplayTags::Character_Debuff_Stun).AddLambda(
				[this](const FGameplayTag EventTag, int32 Count) -> void
				{
					if (Count > 0)
					{
						bIsStunned = true;
					}
					else
					{
						bIsStunned = false;
					}
				});
		}
	}
}

void ULuosCharacterMovementComponent::Strafe()
{
	bIsStrafed = true;
}

void ULuosCharacterMovementComponent::UnStrafe()
{
	bIsStrafed = false;
}

bool ULuosCharacterMovementComponent::IsStrafing() const
{
	return bIsStrafed;
}

void ULuosCharacterMovementComponent::Sprint()
{
	bIsSprinted = true;
}

void ULuosCharacterMovementComponent::UnSprint()
{
	bIsSprinted = false;
}

bool ULuosCharacterMovementComponent::IsSprinting() const
{
	return bIsSprinted;
}
