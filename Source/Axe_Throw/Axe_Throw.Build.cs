// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Axe_Throw : ModuleRules
{
	public Axe_Throw(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
