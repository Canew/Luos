


#include "Player/LuosPlayerController.h"
#include "Player/LuosPlayerState.h"
#include "Engine/LocalPlayer.h"
#include "Input/LuosEnhancedInputComponent.h"
#include "Input/LuosInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "GameplayAbility/LuosGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "UI/LuosHUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosPlayerController)

#define PLAYER_TEAM 1

ALuosPlayerController::ALuosPlayerController()
{
	bAttachToPawn = true;
	bReplicates = true;
	
	SetGenericTeamId(PLAYER_TEAM);
}

void ALuosPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALuosPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerCameraManager->bClientSimulatingViewTarget = false;
	PlayerCameraManager->SetReplicates(true);
	PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 5.0f, FColor::Black);
}

void ALuosPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer());
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EnhancedInputLocalSubsystem);

	for (const UInputMappingContext* InputMappingContext : InputMappings)
	{
		FModifyContextOptions Options = {};
		Options.bIgnoreAllPressedKeysUntilRelease = false;
		EnhancedInputLocalSubsystem->AddMappingContext(InputMappingContext, 0, Options);
	}

	if (ULuosEnhancedInputComponent* LuosEnhancedInputComponent = Cast<ULuosEnhancedInputComponent>(InputComponent))
	{
		for (const ULuosInputConfig* InputConfig : InputConfigs)
		{

		}
	}

	// Register input mapping to user settings
	if (UEnhancedInputUserSettings* EnhancedInputUserSettings = EnhancedInputLocalSubsystem->GetUserSettings())
	{
		for (UInputMappingContext* UserSettingsInputMapping : UserSettingsInputMappings)
		{
			if (!EnhancedInputUserSettings->IsMappingContextRegistered(UserSettingsInputMapping))
			{
				EnhancedInputUserSettings->RegisterInputMappingContext(UserSettingsInputMapping);
			}
		}
	}

	// Initialize input user settings
	UEnhancedInputUserSettings* InputUserSettings = EnhancedInputLocalSubsystem->GetUserSettings();
	if (InputUserSettings)
	{
		if (UEnhancedPlayerMappableKeyProfile* CurrentPlayerKeyProfile = InputUserSettings->GetCurrentKeyProfile())
		{
			for (const TPair<FName, FKeyMappingRow>& PlayerMappingRow : CurrentPlayerKeyProfile->GetPlayerMappingRows())
			{
				FKeyMappingRow KeyMappingRow = PlayerMappingRow.Value;
				for (const FPlayerKeyMapping& PlayerKeyMapping : KeyMappingRow.Mappings.Array())
				{
					if (PlayerKeyMapping.GetSlot() == EPlayerMappableKeySlot::First)
					{
						FMapPlayerKeyArgs MapPlayerKeyArgs;
						MapPlayerKeyArgs.MappingName = PlayerKeyMapping.GetMappingName();
						MapPlayerKeyArgs.NewKey = PlayerKeyMapping.GetCurrentKey();
						MapPlayerKeyArgs.Slot = EPlayerMappableKeySlot::First;
						FGameplayTagContainer FailureReasonTags;

						InputUserSettings->MapPlayerKey(MapPlayerKeyArgs, FailureReasonTags);
					}
				}
			}
		}

		InputUserSettings->ApplySettings();
		InputUserSettings->SaveSettings();
	}
}

void ALuosPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ALuosPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (ULuosAbilitySystemComponent* LuosAbilitySystemComponent = GetLuosAbilitySystemComponent())
	{
		LuosAbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ULuosAbilitySystemComponent* ALuosPlayerController::GetLuosAbilitySystemComponent() const
{
	const ALuosPlayerState* LuosPlayerState = GetPlayerState<ALuosPlayerState>();

	return (LuosPlayerState ? LuosPlayerState->GetAbilitySystemComponent<ULuosAbilitySystemComponent>() : nullptr);
}

FVector ALuosPlayerController::GetMouseDirection() const
{
	if (GetPawn())
	{
		FHitResult HitResult;
		if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
		{
			FVector Direction = HitResult.Location - GetPawn()->GetActorLocation();
			Direction.Normalize();

			return Direction;
		}
	}

	return FVector::ZeroVector;
}

FVector2D ALuosPlayerController::GetMovementInputDirection() const
{
	int AxisX = 0;
	int AxisY = 0;

	if (IsInputKeyDown(EKeys::W))
	{
		AxisX++;
	}
	if (IsInputKeyDown(EKeys::S))
	{
		AxisX--;
	}
	if (IsInputKeyDown(EKeys::A))
	{
		AxisY--;
	}
	if (IsInputKeyDown(EKeys::D))
	{
		AxisY++;
	}

	FRotator BaseRotation = FRotator::ZeroRotator;
	if (GetPawn())
	{
		UCameraComponent* PawnCameraComponent = GetPawn()->GetComponentByClass<UCameraComponent>();
		if (PawnCameraComponent)
		{
			BaseRotation = PawnCameraComponent->GetForwardVector().Rotation();
		}
		else
		{
			BaseRotation = GetPawn()->GetActorForwardVector().Rotation();
		}
	}

	BaseRotation.Roll = 0.0f;
	BaseRotation.Pitch = 0.0f;
	FVector Direction = (BaseRotation.RotateVector(FVector::ForwardVector) * AxisX) + (BaseRotation.RotateVector(FVector::RightVector) * AxisY);
	return FVector2D(Direction.X, Direction.Y);
}

void ALuosPlayerController::CreateBossHealthDisplayActor_Implementation(AActor* NewBossActor)
{
	ALuosHUD* HUD = GetHUD<ALuosHUD>();
	if (HUD)
	{
		HUD->CreateBossHealthDisplayActor(NewBossActor);
	}
}

void ALuosPlayerController::RemoveBossHealthDisplayActor_Implementation()
{
	ALuosHUD* HUD = GetHUD<ALuosHUD>();
	if (HUD)
	{
		HUD->RemoveBossHealthDisplayActor();
	}
}

void ALuosPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	if (TeamId != NewTeamId)
	{
		TeamId = NewTeamId;
	}
}

FGenericTeamId ALuosPlayerController::GetGenericTeamId() const
{
	return TeamId;
}
