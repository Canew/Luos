


#include "Character/LuosCharacter.h"
#include "Character/LuosCharacterSettings.h"
#include "Character/LuosCharacterMovementComponent.h"
#include "Game/LuosGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/LuosPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/LuosEnhancedInputComponent.h"
#include "Input/LuosInputConfig.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "GameplayAbility/LuosGameplayTags.h"
#include "GameplayAbility/LuosAttributeSet.h"
#include "MotionWarpingComponent.h"
#include "AI/LuosAIController.h"
#include "BrainComponent.h"
#include "UI/LuosUserWidget_HealthDisplay.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosCharacter)

FName ALuosCharacter::SpringArmComponentName = TEXT("SpringArm");
FName ALuosCharacter::CameraComponentName = TEXT("Camera");
FName ALuosCharacter::HealthDisplayWidgetComponentName = TEXT("HealthDisplay");
FName ALuosCharacter::MotionWarpingComponentName = TEXT("MotionWarping");

// Sets default values
ALuosCharacter::ALuosCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULuosCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 1024.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.5f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1280.0f, 0.0f);
	GetCharacterMovement()->UseAccelerationForPathFollowing();

	SpringArmComponent = CreateOptionalDefaultSubobject<USpringArmComponent>(SpringArmComponentName);
	SpringArmComponent->TargetOffset = FVector(-40.0f, 0.0f, 0.0f);
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->TargetArmLength = 1400.0f;
	SpringArmComponent->bEnableCameraLag = false;
	SpringArmComponent->bEnableCameraRotationLag = false;
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	CameraComponent = CreateOptionalDefaultSubobject<UCameraComponent>(CameraComponentName);
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthDisplayWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthDisplay"));
	HealthDisplayWidgetComponent->SetWidgetClass(ULuosUserWidget_HealthDisplay::StaticClass());
	HealthDisplayWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthDisplayWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthDisplayWidgetComponent->SetDrawAtDesiredSize(true);
	HealthDisplayWidgetComponent->SetupAttachment(GetCapsuleComponent());
	HealthDisplayWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	HealthDisplayWidgetComponent->SetVisibility(false);
	HealthDisplayWidgetDuration = 4.0f;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(MotionWarpingComponentName);

	AIControllerClass = ALuosAIController::StaticClass();
}

void ALuosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer());
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EnhancedInputSubsystem);

	if (CharacterSettings)
	{
		if (const ULuosInputConfig* InputConfig = CharacterSettings->InputConfig)
		{
			for (const UInputMappingContext* InputMappingContext : InputMappings)
			{
				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;
				EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0, Options);
			}

			if (ULuosEnhancedInputComponent* LuosEnhancedInputComponent = Cast<ULuosEnhancedInputComponent>(PlayerInputComponent))
			{
				LuosEnhancedInputComponent->AddInputMappings(InputConfig, EnhancedInputSubsystem);

				TArray<uint32> BindHandles;
				LuosEnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased, BindHandles);

				LuosEnhancedInputComponent->BindNativeAction(InputConfig, LuosGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
			}
		}
	}
}

void ALuosCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ALuosPlayerState* LuosPlayerState = GetPlayerState<ALuosPlayerState>())
	{
		if (ULuosAbilitySystemComponent* AbilitySystemComponent = LuosPlayerState->GetAbilitySystemComponent<ULuosAbilitySystemComponent>())
		{
			AbilitySystemComponent->InitAbilityActorInfo(LuosPlayerState, this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HealthAttributeEventCallback);

			ULuosCharacterMovementComponent* CharacterMovementComponent = Cast<ULuosCharacterMovementComponent>(GetCharacterMovement());
			if (CharacterMovementComponent)
			{
				CharacterMovementComponent->RegisterGameplayEvent();	
			}
		}

		if (CharacterSettings)
		{
			LuosPlayerState->SetCharacterSettings(CharacterSettings);
		}

		UMaterialInstanceDynamic* MaterialInstanceDynamic = GetMesh()->CreateDynamicMaterialInstance(0);
		if (MaterialInstanceDynamic)
		{
			MaterialInstanceDynamic->SetVectorParameterValue(TEXT("Hair"), LuosPlayerState->GetHairColor());
			MaterialInstanceDynamic->SetVectorParameterValue(TEXT("InnerCloth"), LuosPlayerState->GetInnerClothColor());
			MaterialInstanceDynamic->SetVectorParameterValue(TEXT("OuterCloth"), LuosPlayerState->GetOuterClothColor());
		}
	}
}

FString ALuosCharacter::GetHumanReadableName() const
{
	if (CharacterSettings)
	{
		return CharacterSettings->CharacterName.ToString();
	}

	return Super::GetHumanReadableName();
}

void ALuosCharacter::BeginPlay()
{
	Super::BeginPlay();

	ULuosUserWidget_HealthDisplay* HealthDisplayWidget = Cast<ULuosUserWidget_HealthDisplay>(HealthDisplayWidgetComponent->GetUserWidgetObject());
	if (HealthDisplayWidget)
	{
		HealthDisplayWidgetComponent->InitWidget();
		HealthDisplayWidget->SetOwningActor(this);
	}
}

void ALuosCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(FlickHitFeedbackMaterialTimerHandle);
	GetWorldTimerManager().ClearTimer(ShowHealthWidgetTimerHandle);
}

void ALuosCharacter::OnRep_PlayerState()
{
	if (ALuosPlayerState* LuosPlayerState = GetPlayerState<ALuosPlayerState>())
	{
		if (ULuosAbilitySystemComponent* AbilitySystemComponent = LuosPlayerState->GetAbilitySystemComponent<ULuosAbilitySystemComponent>())
		{
			AbilitySystemComponent->InitAbilityActorInfo(LuosPlayerState, this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ULuosAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HealthAttributeEventCallback);

			ULuosCharacterMovementComponent* CharacterMovementComponent = Cast<ULuosCharacterMovementComponent>(GetCharacterMovement());
			if (CharacterMovementComponent)
			{
				CharacterMovementComponent->RegisterGameplayEvent();
			}
		}

		UMaterialInstanceDynamic* MaterialInstanceDynamic = GetMesh()->CreateDynamicMaterialInstance(0);
		if (MaterialInstanceDynamic)
		{
			MaterialInstanceDynamic->SetVectorParameterValue(TEXT("Hair"), LuosPlayerState->GetHairColor());
			MaterialInstanceDynamic->SetVectorParameterValue(TEXT("InnerCloth"), LuosPlayerState->GetInnerClothColor());
			MaterialInstanceDynamic->SetVectorParameterValue(TEXT("OuterCloth"), LuosPlayerState->GetOuterClothColor());
		}

		ULuosUserWidget_HealthDisplay* HealthDisplayWidget = Cast<ULuosUserWidget_HealthDisplay>(HealthDisplayWidgetComponent->GetUserWidgetObject());
		if (HealthDisplayWidget)
		{
			HealthDisplayWidgetComponent->InitWidget();
			HealthDisplayWidget->SetOwningActor(this);
		}
	}
}

UAbilitySystemComponent* ALuosCharacter::GetAbilitySystemComponent() const
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (AbilitySystemInterface)
	{
		return AbilitySystemInterface->GetAbilitySystemComponent();
	}

	return nullptr;
}

void ALuosCharacter::Turn(const FVector& Direction, bool bTurnInstantly)
{
	InternalTurn(Direction, bTurnInstantly);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerTurn(Direction, bTurnInstantly);
	}
}

void ALuosCharacter::InternalTurn(const FVector& Direction, bool bTurnInstantly)
{
	check(GetCharacterMovement());
	FRotator CurrentRotation = GetActorForwardVector().Rotation();
	FRotator TargetRotation = Direction.IsZero() ? GetActorForwardVector().Rotation() : Direction.Rotation();
	TargetRotation.Roll = 0.0f;
	TargetRotation.Pitch = 0.0f;

	if (bTurnInstantly)
	{
		SetActorRotation(TargetRotation);
	}
	else
	{
		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), GetCharacterMovement()->RotationRate.Yaw);
		SetActorRotation(NewRotation);
	}
}

void ALuosCharacter::ServerTurn_Implementation(const FVector& Direction, bool bTurnInstantly)
{
	InternalTurn(Direction, bTurnInstantly);
}

void ALuosCharacter::HitFeedback()
{
	if (HasAuthority())
	{
		MulticastHitFeedback();
	}
}

void ALuosCharacter::MulticastHitFeedback_Implementation()
{
	GetMesh()->SetOverlayMaterial(HitFeedbackMaterial);

	FTimerDelegate FlickHitFeedbackMaterialDelegate = FTimerDelegate::CreateLambda([this]() -> void
		{
			if (IsValid(this) && IsValid(GetMesh()))
			{
				GetMesh()->SetOverlayMaterial(nullptr);
			}
		});

	GetWorldTimerManager().SetTimer(FlickHitFeedbackMaterialTimerHandle, FlickHitFeedbackMaterialDelegate, FlickHitFeedbackMaterialTime, false);
}

bool ALuosCharacter::IsAlive() const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (AbilitySystemComponent)
	{
		bool bFound = false;
		float Health = AbilitySystemComponent->GetGameplayAttributeValue(ULuosAttributeSet::GetHealthAttribute(), bFound);
		
		return (bFound && Health > 0.0f);
	}

	return false;
}

void ALuosCharacter::MoveInput(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	FRotator CameraRotation = CameraComponent->GetForwardVector().Rotation();
	CameraRotation.Roll = 0.0f;
	CameraRotation.Pitch = 0.0f;

	if (Value.X != 0.0f)
	{
		const FVector MovementDirection = CameraRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector MovementDirection = CameraRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Value.Y);
	}
}

void ALuosCharacter::AbilityInputPressed(FGameplayTag InputTag)
{
	ULuosAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent<ULuosAbilitySystemComponent>();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void ALuosCharacter::AbilityInputReleased(FGameplayTag InputTag)
{
	ULuosAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent<ULuosAbilitySystemComponent>();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}

FGenericTeamId ALuosCharacter::GetGenericTeamId() const
{
	IGenericTeamAgentInterface* GenericTeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetController());
	if (GenericTeamAgentInterface)
	{
		return GenericTeamAgentInterface->GetGenericTeamId();
	}

	return FGenericTeamId::NoTeam;
}

void ALuosCharacter::HealthAttributeEventCallback(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		DisableInput(GetController<APlayerController>());
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetCapsuleComponent()->SetGenerateOverlapEvents(false);
		GetCharacterMovement()->GravityScale = 0.0f;
		GetCharacterMovement()->StopMovementImmediately();

		// Play death montage. if montage length < 0.0f, play ragdoll.
		StopAnimMontage(GetCurrentMontage());
		float DeathMontageLength = PlayDeathMontage();
		if (DeathMontageLength <= 0.0f)
		{
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetAllBodiesSimulatePhysics(true);

			DeathMontageLength = 1.0f;
		}

		if (HasAuthority())
		{
			// Fade out dead player camera
			if (IsPlayerControlled())
			{
				GetController<APlayerController>()->ClientSetCameraFade(true, FColor::Black, FVector2D(0.0f, 1.0f), 0.5f, true, true);
			}
			else
			{
				AAIController* AIController = GetController<AAIController>();
				if (AIController)
				{
					AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
				}
			}

			// Set finish death montage timer.
			FTimerHandle DeathTimerHandle;
			GetWorldTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda(
				[this]() -> void
				{
					if (this)
					{
						if (IsPlayerControlled())
						{
							ALuosGameMode* GameMode = GetWorld()->GetAuthGameMode<ALuosGameMode>();
							if (GameMode)
							{
								GameMode->ProcessPlayerDeath(GetController<APlayerController>());
							}
						}

						Destroy();
					}
				}), DeathMontageLength, false);
		}
	}
}

float ALuosCharacter::PlayDeathMontage_Implementation()
{
	return 0.0f;
}

void ALuosCharacter::ClientShowHealthDisplayWidget_Implementation(ALuosCharacter* TargetCharacter)
{
	if (IsPlayerControlled())
	{
		TargetCharacter->ShowHealthDisplayWidget();
	}
}

void ALuosCharacter::ShowHealthDisplayWidget()
{
	if (HealthDisplayWidgetComponent)
	{
		HealthDisplayWidgetComponent->SetVisibility(true);
		GetWorldTimerManager().SetTimer(ShowHealthWidgetTimerHandle, FTimerDelegate::CreateLambda(
			[this]() -> void
			{
				if (this && HealthDisplayWidgetComponent)
				{
					HealthDisplayWidgetComponent->SetVisibility(false);
				}
			}), HealthDisplayWidgetDuration, false);
	}
}
