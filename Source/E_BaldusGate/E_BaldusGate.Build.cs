// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class E_BaldusGate : ModuleRules
{
	public E_BaldusGate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG",
			"Slate",  "SlateCore", "Json" , "JsonUtilities"
		});
	}
}
