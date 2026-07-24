// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.Collections.Generic;

public class SyphonPluginTarget : TargetRules
{
	public SyphonPluginTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("SyphonPlugin");
		AdditionalCompilerArguments = "-Wno-unused-command-line-argument";
	}
}
