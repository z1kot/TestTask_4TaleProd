// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TestTask_4Tale : ModuleRules
{
	public TestTask_4Tale(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PublicIncludePaths.AddRange(new string[] { "TestTask_4Tale" });
    }
}
