

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "LuosPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ALuosPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ALuosPlayerController();

	//~APlayerController interface
	virtual void PostInitializeComponents() override;
	virtual void SetupInputComponent() override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

protected:
	virtual void BeginPlay() override;
	//~End of APlayerController interface

public:
	UFUNCTION(BlueprintCallable, Category = "Luos|Ability")
	class ULuosAbilitySystemComponent* GetLuosAbilitySystemComponent() const;

	/** Return a normalized vector that heads from the chracter location towards the mouse position hit result location. */
	UFUNCTION(BlueprintCallable, Category = "Luos|Controller")
	FVector GetMouseDirection() const;

	/** Return a normalized vector of the direction of movement input by the player. */
	UFUNCTION(BlueprintCallable, Category = "Luos|Controller")
	FVector2D GetMovementInputDirection() const;

	UFUNCTION(Client, Reliable)
	void CreateBossHealthDisplayActor(AActor* NewBossActor);
	UFUNCTION(Client, Reliable)
	void RemoveBossHealthDisplayActor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<class UInputMappingContext>> InputMappings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<class ULuosInputConfig>> InputConfigs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<class UInputMappingContext>> UserSettingsInputMappings;

#pragma region GenericTeamAgentInterface
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	UPROPERTY()
	FGenericTeamId TeamId;

#pragma endregion
};
