// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VesselSimulation : ModuleRules
{
	public VesselSimulation(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
