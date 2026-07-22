// Copyright 2026 Yoshitsugu Kosaka. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class SyphonLink : ModuleRules
{
    public SyphonLink(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
            { "Core", "CoreUObject", "Engine", "RHI", "RenderCore","CinematicCamera" });

        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.AddRange(new string[] { "Metal", "IOSurface" });

            string SyphonPath = Path.Combine(ModuleDirectory, "ThirdParty", "Syphon");
            string FrameworkPath = Path.Combine(SyphonPath, "Syphon.framework");

            PublicIncludePaths.Add(Path.Combine(SyphonPath, "include"));
            PublicAdditionalLibraries.Add(Path.Combine(FrameworkPath, "Versions", "A", "Syphon"));

            // Ship the framework inside packaged builds
            RuntimeDependencies.Add(
                "$(BinaryOutputDir)/Syphon.framework",
                Path.Combine(FrameworkPath, "*"),
                StagedFileType.NonUFS);
        }
    }
}