// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP_S : ModuleRules
{
	public TP_S(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags", "GameplayAbilities", "GameplayTasks", "NavigationSystem", "AIModule", "AnimGraphRuntime", "MotionWarping" });
	}
}
