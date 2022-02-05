// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestGame2 : ModuleRules
{
	public TestGame2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Landscape" });
	}
}
