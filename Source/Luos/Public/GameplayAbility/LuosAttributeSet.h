

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LuosAttributeSet.generated.h"

/**
 * macro defines a set of helper functions for accessing and initializing attributes.
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class LUOS_API ULuosAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	ULuosAttributeSet();

	UWorld* GetWorld() const override;

	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, AttackPower);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, AttackSpeed);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, MovementSpeed);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, Resistance);

	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(ULuosAttributeSet, Damage);

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

protected:
	// The current health attribute. The health will be capped by the max health attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Luos|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Luos|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// The current mana attribute. The mana will be capped by the max mana attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Luos|Mana", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Mana;

	// The current max mana attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Luos|Mana", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMana;

	// The current stamina attribute. The stamina will be capped by the max stamina attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Luos|Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	// The current max stamina attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Luos|Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "Luos|Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Luos|Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Luos|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Luos|Defense", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Armor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resistance, Category = "Luos|Defense", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resistance;


	/* 	Meta Attribute(please keep attributes that aren't 'stateful' below */

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category = "Luos|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category = "Luos|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
};
