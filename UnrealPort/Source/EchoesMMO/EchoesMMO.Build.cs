using UnrealBuildTool;

public class EchoesMMO : ModuleRules
{
	public EchoesMMO(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"UMG", "Slate", "SlateCore", 
			"OnlineSubsystem", "OnlineSubsystemUtils"
		});
	}
}