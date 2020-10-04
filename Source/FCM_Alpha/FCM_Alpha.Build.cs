// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FCM_Alpha : ModuleRules
{
	public FCM_Alpha(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
