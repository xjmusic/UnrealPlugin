// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

using System.IO;
using System.Text.RegularExpressions;
using UnrealBuildTool;

public class XJMusicPlugin : ModuleRules
{
	public XJMusicPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
        CppStandard = CppStandardVersion.Cpp17;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Projects",
				"InputCore",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "AudioMixer",
                "AudioExtensions"
            }
		);

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../xj/engine/include/"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../xj/engine/build/_deps/json-src/include"));

        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../xj/engine/build/src/Release/xjmusic.lib"));
    }
}
