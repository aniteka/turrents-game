// TurretGame by Team #1. AlphaNova courses

using UnrealBuildTool;
using System.Collections.Generic;

public class TurretsGameEditorTarget : TargetRules
{
	public TurretsGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TurretsGame" } );
	}
}
