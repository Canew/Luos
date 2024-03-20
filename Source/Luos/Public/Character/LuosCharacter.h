

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameplayEffectTypes.h"
#include "LuosCharacter.generated.h"

UCLASS()
class LUOS_API ALuosCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ALuosCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual FString GetHumanReadableName() const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRep_PlayerState() override;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	template <typename T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	void Turn(const FVector& Direction, bool bTurnInstantly = false);
	void InternalTurn(const FVector& Direction, bool bTurnInstantly = false);

	UFUNCTION(Server, Reliable)
	void ServerTurn(const FVector& Direction, bool bTurnInstantly = false);

	bool IsAlive() const;

protected:
	void MoveInput(const FInputActionValue& InputActionValue);

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

	void HealthAttributeEventCallback(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintNativeEvent, Category = "Luos|Character")
	float PlayDeathMontage();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	static FName SpringArmComponentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComponent;
	static FName CameraComponentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;
	static FName MotionWarpingComponentName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<class UInputMappingContext>> InputMappings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<class ULuosCharacterSettings> CharacterSettings;

#pragma region HitFeedback
public:
	/** Play visual effect when character get damaged (flick mesh material, etc.) */
	UFUNCTION()
	virtual void HitFeedback();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHitFeedback();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Luo|Hit Feedback")
	TObjectPtr<UMaterialInterface> HitFeedbackMaterial;

	FTimerHandle FlickHitFeedbackMaterialTimerHandle;
	float FlickHitFeedbackMaterialTime = 0.025f;

#pragma endregion

#pragma region HealthDisplay
public:
	UFUNCTION(Client, Reliable)
	virtual void ClientShowHealthDisplayWidget(ALuosCharacter* TargetCharacter);
	virtual void ShowHealthDisplayWidget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UWidgetComponent> HealthDisplayWidgetComponent;
	static FName HealthDisplayWidgetComponentName;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	FName HealthDisplayWidgetAttachSocketName = TEXT("HealthDisplay");

	FTimerHandle ShowHealthWidgetTimerHandle;
	float HealthDisplayWidgetDuration;

#pragma endregion
};
