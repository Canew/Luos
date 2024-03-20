


#include "Input/LuosInputConfig.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosInputConfig)

ULuosInputConfig::ULuosInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* ULuosInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FLuosInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}

const UInputAction* ULuosInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FLuosInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
