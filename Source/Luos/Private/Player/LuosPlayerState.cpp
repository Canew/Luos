


#include "Player/LuosPlayerState.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"
#include "GameplayAbility/LuosAttributeSet.h"
#include "GameplayAbility/LuosAbilitySet.h"
#include "GameplayAbility/LuosGameplayAbility.h"
#include "Player/LuosPlayerController.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosPlayerState)

ALuosPlayerState::ALuosPlayerState(const FObjectInitializer& ObjectInitializer)
{
	SetReplicates(true);

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULuosAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CreateDefaultSubobject<ULuosAttributeSet>(TEXT("AttributeSet"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void ALuosPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALuosPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);

	if (AActor* AvatarActor = GetPawn())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, AvatarActor);
	}
}

void ALuosPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALuosPlayerState, HairColor);
	DOREPLIFETIME(ALuosPlayerState, InnerClothColor);
	DOREPLIFETIME(ALuosPlayerState, OuterClothColor);
	DOREPLIFETIME(ALuosPlayerState, bIsReadied);
}

void ALuosPlayerState::OverrideWith(APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);

	ALuosPlayerState* LuosPlayerState = Cast<ALuosPlayerState>(PlayerState);
	if (LuosPlayerState)
	{
		LuosPlayerState->HairColor = HairColor;
		LuosPlayerState->InnerClothColor = InnerClothColor;
		LuosPlayerState->OuterClothColor =OuterClothColor;
	}
}

void ALuosPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	ALuosPlayerState* LuosPlayerState = Cast<ALuosPlayerState>(PlayerState);
	if (LuosPlayerState)
	{
		LuosPlayerState->HairColor = HairColor;
		LuosPlayerState->InnerClothColor = InnerClothColor;
		LuosPlayerState->OuterClothColor = OuterClothColor;
	}
}

ALuosPlayerController* ALuosPlayerState::GetLuosPlayerController() const
{
	return Cast<ALuosPlayerController>(GetOwner());
}

UAbilitySystemComponent* ALuosPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALuosPlayerState::SetCharacterSettings(const ULuosCharacterSettings* InCharacterSettings)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (!InCharacterSettings || CharacterSettings)
	{
		return;
	}

	CharacterSettings = InCharacterSettings;

	for (const ULuosAbilitySet* AbilitySet : CharacterSettings->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	CharacterSettingsChanged.Broadcast(InCharacterSettings);
}

FLinearColor ALuosPlayerState::GetHairColor()
{
	return HairColor;
}

FLinearColor ALuosPlayerState::GetInnerClothColor()
{
	return InnerClothColor;
}

FLinearColor ALuosPlayerState::GetOuterClothColor()
{
	return OuterClothColor;
}

void ALuosPlayerState::SetHairColor_Implementation(FLinearColor Color)
{
	HairColor = Color;
}

void ALuosPlayerState::SetInnerClothColor_Implementation(FLinearColor Color)
{
	InnerClothColor = Color;
}

void ALuosPlayerState::SetOuterClothColor_Implementation(FLinearColor Color)
{
	OuterClothColor = Color;
}

bool ALuosPlayerState::IsReadied()
{
	return bIsReadied;
}

void ALuosPlayerState::Ready_Implementation()
{
	bIsReadied = true;
}

void ALuosPlayerState::UnReady_Implementation()
{
	bIsReadied = false;
}