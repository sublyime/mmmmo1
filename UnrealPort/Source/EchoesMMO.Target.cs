using UnrealBuildTool;
using System.Collections.Generic;

public class EchoesMMOTarget : TargetRules
{
	public EchoesMMOTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange(new string[] { "EchoesMMO" });
	}
}
