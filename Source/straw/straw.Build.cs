// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class straw : ModuleRules
{
	public straw(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ProceduralMeshComponent", "Niagara", "AdvancedWidgets" });
	}
}
