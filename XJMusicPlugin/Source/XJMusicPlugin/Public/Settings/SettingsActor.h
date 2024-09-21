// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SettingsActor.generated.h"

USTRUCT(BlueprintType)
struct FXjSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (AllowedClasses = "XjProject"))
	FSoftObjectPath LaunchProject;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bDefaultOutput = true;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bShowDebugTimeline = true;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<FString> DefaultTaxonomies;
};

UCLASS()
class ASettingsActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASettingsActor();

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bOverrideLaunchProject = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (AllowedClasses = "XjProject", EditCondition = "bOverrideLaunchProject"))
	FSoftObjectPath LaunchProject;


	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bOverrideDefaultOutput = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "bOverrideDefaultOutput"))
	bool bDefaultOutput = true;


	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bOverrideShowDebugTimeline = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "bOverrideShowDebugTimeline"))
	bool bShowDebugTimeline = true;


	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bOverrideDefaultTaxonomies = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition = "bOverrideDefaultTaxonomies"))
	TArray<FString> DefaultTaxonomies;

	FXjSettings GetSettingsOverride(const FXjSettings& Base) const
	{
		FXjSettings Override = Base;

		if (bOverrideLaunchProject)
		{
			Override.LaunchProject = LaunchProject;
		}

		if (bOverrideDefaultOutput)
		{
			Override.bDefaultOutput = bDefaultOutput;
		}

		if (bOverrideShowDebugTimeline)
		{
			Override.bShowDebugTimeline = bShowDebugTimeline;
		}

		if (bOverrideDefaultTaxonomies)
		{
			Override.DefaultTaxonomies = DefaultTaxonomies;
		}

		return Override;
	}
};
