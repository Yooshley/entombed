// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class entombed : ModuleRules
{
	public entombed(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",

			// Input
			"InputCore",
			"EnhancedInput",

			// UI
			"UMG",
			"Slate",
			"SlateCore",

			// MVVM
			"ModelViewViewModel",

			// GAS
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NavigationSystem",
			"Niagara",
			"AIModule"
		});
	}
}