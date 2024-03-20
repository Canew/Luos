


#include "GameplayAbility/LuosAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Character/LuosCharacter.h"
#include "Player/LuosPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosAttributeSet)

ULuosAttributeSet::ULuosAttributeSet()
	: Health(1.0f)
	, MaxHealth(1.0f)
	, Mana(0.0f)
	, MaxMana(0.0f)
	, Stamina(0.0f)
	, MaxStamina(0.0f)
	, AttackPower(0.0f)
	, AttackSpeed(1.0f)
	, MovementSpeed(100.0f)
	, Armor(0.0f)
	, Resistance(0.0f)
{
}

void ULuosAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULuosAttributeSet, Resistance, COND_None, REPNOTIFY_Always);
}

UWorld* ULuosAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

void ULuosAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, Health, OldValue);
}

void ULuosAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, MaxHealth, OldValue);
}

void ULuosAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, Mana, OldValue);
}

void ULuosAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, MaxMana, OldValue);
}

void ULuosAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, Stamina, OldValue);
}

void ULuosAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, MaxStamina, OldValue);
}

void ULuosAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, AttackPower, OldValue);
}

void ULuosAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, AttackSpeed, OldValue);
}

void ULuosAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, MovementSpeed, OldValue);
}

void ULuosAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, Armor, OldValue);
}

void ULuosAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULuosAttributeSet, Resistance, OldValue);
}

bool ULuosAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void ULuosAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ALuosCharacter* SourceCharacter = Cast<ALuosCharacter>(Data.EffectSpec.GetEffectContext().GetEffectCauser());
		ALuosCharacter* TargetCharacter = Cast<ALuosCharacter>(Data.Target.GetAvatarActor());

		// Convert into -Health and then clamp
		float DamageDone = GetDamage() - GetArmor();

		if (IsValid(SourceCharacter))
		{
			SourceCharacter->ClientShowHealthDisplayWidget(TargetCharacter);
		}

		if (DamageDone > 0.0f)
		{
			SetHealth(FMath::Clamp(GetHealth() - DamageDone, 0.0f, GetMaxHealth()));

			if (IsValid(TargetCharacter))
			{
				TargetCharacter->HitFeedback();
			}
		}

		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Convert into +Health and then clamo
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), 0.0f, GetMaxHealth()));
		SetHealing(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
	{
		SetAttackPower(FMath::Clamp(GetAttackPower(), 0.0f, GetAttackPower()));
	}
	else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute())
	{
		SetAttackSpeed(FMath::Clamp(GetAttackSpeed(), 0.0f, GetAttackSpeed()));
	}
	else if (Data.EvaluatedData.Attribute == GetMovementSpeedAttribute())
	{
		SetMovementSpeed(FMath::Clamp(GetMovementSpeed(), 100.0f, GetMovementSpeed()));
	}
	else if (Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(FMath::Clamp(GetArmor(), 100.0f, GetArmor()));
	}
	else if (Data.EvaluatedData.Attribute == GetResistanceAttribute())
	{
		SetResistance(FMath::Clamp(GetResistance(), 100.0f, GetResistance()));
	}
}

void ULuosAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetAttackPowerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, NewValue);
	}
	else if (Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, NewValue);
	}
	else if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 100.0f, NewValue);
	}
	else if (Attribute == GetArmorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, NewValue);
	}
	else if (Attribute == GetResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, NewValue);
	}
}

void ULuosAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetHealth() > NewValue)
		{
			UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
			check(AbilitySystemComponent);

			AbilitySystemComponent->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetMana() > NewValue)
		{
			UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
			check(AbilitySystemComponent);

			AbilitySystemComponent->ApplyModToAttribute(GetManaAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetStamina() > NewValue)
		{
			UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
			check(AbilitySystemComponent);

			AbilitySystemComponent->ApplyModToAttribute(GetStaminaAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

void ULuosAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void ULuosAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}
