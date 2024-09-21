// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types/XjProject.h"
#include "SettingsActor.h"
#include "XJMusicDefaultSettings.generated.h"

UCLASS(config = "XJMusicConfig")
class XJMUSICPLUGIN_API UXJMusicDefaultSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	FXjSettings SettingsObject;
	
	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	FString PathToXjMusicWorkstation;
};
