// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types/XjProject.h"
#include "XJMusicDefaultSettings.generated.h"

UCLASS(config = "XJMusicConfig")
class XJMUSICPLUGIN_API UXJMusicDefaultSettings : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Config, EditAnywhere, Category = "Settings", meta=(AllowedClasses="XjProject"))
	FSoftObjectPath LaunchProject;
	
	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	bool bDefaultOutput = true;

	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	bool bShowDebugTimeline = true;

	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	TArray<FString> DefaultTaxonomies;
	
	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	FString PathToXjMusicWorkstation;

	UPROPERTY(EditAnywhere, Category = "Development")
	bool bDevelopmentMode = false;

	UPROPERTY(EditAnywhere, Category = "Development", meta = (EditCondition = "bDevelopmentMode"))
	class UDataTable* MockDataDT = nullptr;

	UPROPERTY(EditAnywhere, Category = "Development", meta = (EditCondition = "bDevelopmentMode"))
	int MaxAudiosOutputPerCycle = 0;

	UPROPERTY(EditAnywhere, Category = "Development", meta = (EditCondition = "bDevelopmentMode"))
	float LatencyBetweenCyclesInSeconds = 0.0f;

};
