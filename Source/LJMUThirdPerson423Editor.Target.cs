// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LJMUThirdPerson423EditorTarget : TargetRules
{
	public LJMUThirdPerson423EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("LJMUThirdPerson423");
	}
}
