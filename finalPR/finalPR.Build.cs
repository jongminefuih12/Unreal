// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class finalPR : ModuleRules
{
	public finalPR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
