

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LuosCharacterSettings.generated.h"

/**
 * 
 */
UCLASS()
class LUOS_API ULuosCharacterSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
	FText CharacterName;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TObjectPtr<class ULuosAbilitySet>> AbilitySets;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class ULuosInputConfig> InputConfig;
};
