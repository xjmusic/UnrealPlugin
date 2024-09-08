// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XJGame : ModuleRules
{
	public XJGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        OptimizeCode = CodeOptimization.Never;
    }
}
