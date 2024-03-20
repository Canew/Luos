


#include "GameplayAbility/LuosGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace LuosGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Ability, "Ability");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Property, "Ability.Property");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Property_Cancellable, "Ability.Property.Cancellable");

	UE_DEFINE_GAMEPLAY_TAG(Character, "Character");
	UE_DEFINE_GAMEPLAY_TAG(Character_Strafe, "Character.Strafe");
	UE_DEFINE_GAMEPLAY_TAG(Character_Sprint, "Character.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Character_Evade, "Character.Evade");
	UE_DEFINE_GAMEPLAY_TAG(Character_Debuff_Stun, "Character.Debuff.Stun");

	UE_DEFINE_GAMEPLAY_TAG(Input, "Input");
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move");

	UE_DEFINE_GAMEPLAY_TAG(Data, "Data");
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Data_Force, "Data.Force");
	UE_DEFINE_GAMEPLAY_TAG(Data_Duration, "Data.Duration");
	UE_DEFINE_GAMEPLAY_TAG(Data_Cooldown, "Data.Cooldown");

	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent, "GameplayEvent");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Collision, "GameplayEvent.Collision");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Cancellable, "GameplayEvent.Cancellable");

	UE_DEFINE_GAMEPLAY_TAG(GameplayEffect, "GameplayEffect");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEffect_Damage, "GameplayEffect.Damage");
}