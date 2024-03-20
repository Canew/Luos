// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Luos : ModuleRules
{
	public Luos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CommonUI" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });
        PrivateDependencyModuleNames.AddRange(new string[] { "MotionWarping" });
        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTasks", "GameplayTags" });
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam" });
        PrivateDependencyModuleNames.AddRange(new string[] { "AdvancedSessions", "AdvancedSteamSessions" });
    }
}
