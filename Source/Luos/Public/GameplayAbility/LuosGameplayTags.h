

#pragma once

#include "NativeGameplayTags.h"

namespace LuosGameplayTags
{
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Property);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Property_Cancellable);

	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Strafe);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Sprint);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Evade);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Debuff_Stun);

	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Move);

	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Force);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Duration);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Cooldown);

	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Collision);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Cancellable);

	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEffect);
	LUOS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEffect_Damage);
}