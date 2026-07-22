// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.Collections.Generic;

public class SyphonePluginTarget : TargetRules
{
	public SyphonePluginTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("SyphonePlugin");
		AdditionalCompilerArguments = "-Wno-unused-command-line-argument";
	}
}
