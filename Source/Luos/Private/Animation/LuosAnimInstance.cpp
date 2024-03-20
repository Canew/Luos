


#include "Animation/LuosAnimInstance.h"
#include "GameFramework/Character.h"
#include "Character/LuosCharacterMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAnimInstance)

void ULuosAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	InitializeProperties();
}

void ULuosAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	FName FullBodyCurveName = TEXT("FullBody");
	bFullBody = GetCurveValue(FullBodyCurveName) > KINDA_SMALL_NUMBER;

	if (Character.IsValid() && CharacterMovementComponent.IsValid())
	{
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = Velocity.Size();

		FVector CurrentAcceleration = CharacterMovementComponent->GetCurrentAcceleration();
		ShouldMove = (!CurrentAcceleration.IsNearlyZero()) && (GroundSpeed > 1.0f);

		IsStrafing = CharacterMovementComponent->IsStrafing();
		IsSprinting = CharacterMovementComponent->IsSprinting();
		IsFalling = CharacterMovementComponent->IsFalling();
		if (IsFalling)
		{
			SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		}
		else
		{
			SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		}

		FVector ForwardVector = FVector(Character->GetActorForwardVector().X, Character->GetActorForwardVector().Y, 0.0f);
		FVector MoveVector = FVector(Character->GetVelocity().X, Character->GetVelocity().Y, 0.0f);
		ForwardVector.Normalize();
		MoveVector.Normalize();
		if (GroundSpeed > 0.0f)
		{
			MoveDirection = FMath::RadiansToDegrees(FMath::Acos(ForwardVector | MoveVector));
		}

		float DistanceToFrontBackPlane = FVector::PointPlaneDist(Character->GetActorLocation() + Character->GetVelocity(), Character->GetActorLocation(), Character->GetActorRightVector());
		if (DistanceToFrontBackPlane < 0)
		{
			MoveDirection *= -1.0f;
		}
	}
}

void ULuosAnimInstance::PushDisableRootMotion(UAnimMontage* Montage, float Duration)
{
	FAnimMontageInstance* AnimMontageInstance = GetActiveInstanceForMontage(Montage);
	if (AnimMontageInstance)
	{
		AnimMontageInstance->PushDisableRootMotion();
		if (Duration > 0.0f)
		{
			FTimerHandle PushDisableRootMotionTimerHandle;
			FTimerDelegate PushDisableRootMotionTimerDelegate;
			PushDisableRootMotionTimerDelegate.BindUObject(this, &ThisClass::PopDisableRootMotion, Montage);
			GetWorld()->GetTimerManager().SetTimer(PushDisableRootMotionTimerHandle, PushDisableRootMotionTimerDelegate, Duration, false);
		}
	}
}

void ULuosAnimInstance::PopDisableRootMotion(UAnimMontage* Montage)
{
	FAnimMontageInstance* AnimMontageInstance = GetActiveInstanceForMontage(Montage);
	if (AnimMontageInstance)
	{
		AnimMontageInstance->PopDisableRootMotion();
	}
}

void ULuosAnimInstance::InitializeProperties()
{
	Character = Cast<ACharacter>(GetOwningActor());
	if (IsValid(Character.Get()))
	{
		bool bSucceeded = false;
		CharacterMovementComponent = Cast<ULuosCharacterMovementComponent>(Character->GetCharacterMovement());
		AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character.Get());
		if (IsValid(CharacterMovementComponent.Get()) && IsValid(AbilitySystemComponent.Get()))
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TEXT("Character.Debuff.Stun"))).AddLambda(
				[this](const FGameplayTag GameplayTag, int32 Count) -> void
				{
					if (Count > 0)
					{
						IsStunned = true;
					}
					else
					{
						IsStunned = false;
					}
				});

			bSucceeded = true;
			Character->GetWorldTimerManager().ClearTimer(InitializeTimerHandle);
		}

		if (!bSucceeded)
		{
			Character->GetWorldTimerManager().SetTimer(InitializeTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::InitializeProperties), 1.0f, true);
		}
	}
}
