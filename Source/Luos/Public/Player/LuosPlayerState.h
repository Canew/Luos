

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "Character/LuosCharacterSettings.h"
#include "LuosPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLuosCharacterSettingsChanged, const ULuosCharacterSettings*, CharacterSettings);

/**
 * 
 */
UCLASS()
class LUOS_API ALuosPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALuosPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void OverrideWith(APlayerState* PlayerState) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Luos|PlayerState")
	class ALuosPlayerController* GetLuosPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Luos|PlayerState")
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	template <typename T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	UFUNCTION(BlueprintPure, Category = "Luos|PlayerState")
	const ULuosCharacterSettings* GetCharacterSetting() const { return CharacterSettings; }
	UFUNCTION(BlueprintCallable, Category = "Luos|PlayerState")
	void SetCharacterSettings(const class ULuosCharacterSettings* InCharacterSettings);

	UFUNCTION(BlueprintPure)
	FLinearColor GetHairColor();
	UFUNCTION(BlueprintPure)
	FLinearColor GetInnerClothColor();
	UFUNCTION(BlueprintPure)
	FLinearColor GetOuterClothColor();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetHairColor(FLinearColor Color);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetInnerClothColor(FLinearColor Color);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetOuterClothColor(FLinearColor Color);

	UFUNCTION(BlueprintPure)
	bool IsReadied();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Ready();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UnReady();

public:
	UPROPERTY(BlueprintAssignable, Category = "Luos|PlayerState")
	FLuosCharacterSettingsChanged CharacterSettingsChanged;

protected:
	UPROPERTY()
	TObjectPtr<const class ULuosCharacterSettings> CharacterSettings;

	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere, Category = "Luos|PlayerState")
	FLinearColor HairColor;
	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere, Category = "Luos|PlayerState")
	FLinearColor InnerClothColor;
	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere, Category = "Luos|PlayerState")
	FLinearColor OuterClothColor;

	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere, Category = "Luos|PlayerState")
	bool bIsReadied;

private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<class ULuosAbilitySystemComponent> AbilitySystemComponent;
};
