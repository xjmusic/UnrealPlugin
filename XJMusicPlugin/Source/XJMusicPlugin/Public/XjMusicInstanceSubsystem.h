// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/DebugChainView.h"
#include "Settings/SettingsActor.h"
#include "XjMusicInstanceSubsystem.generated.h"

class UXjProject;
class UXjManager;
class UXjMixer;
class UXjAudioLoader;
class UXJMusicDefaultSettings;

FXjSettings GetSettings(UWorld* World);

UCLASS(DisplayName = "XjSubsystem")
class XJMUSICPLUGIN_API UXjMusicInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void DoOverrideTaxonomy(const FString Taxonomy);

	UFUNCTION(BlueprintCallable)
	void DoOverrideMacro(const FString Macro);

	UFUNCTION(BlueprintCallable)
	void DoOverrideIntensity(const float Value);

	UFUNCTION(BlueprintPure)
	TArray<FString> GetAllTaxonomyMemes() const;

	UFUNCTION(BlueprintPure)

	TArray<FString> GetAllMacros() const;

	void SetupXJ();

	void ShutdownXJ();

	void AddOrUpdateActiveAudio(const FAudioPlayer& Audio);

	void RemoveActiveAudio(const FAudioPlayer& Audio);
	
	const TMap<FString, FAudioPlayer>& GetActiveAudios() const
	{
		return ActiveAudios;
	}

	FString GetRuntimeProjectDirectory() const
	{
		return RuntimeProjectDir;
	}

	UPROPERTY()
	UXjProject* XjProjectInstance = nullptr;

	UPROPERTY()
	UXjManager* Manager = nullptr;

	UPROPERTY()
	UXjMixer* Mixer = nullptr;

	UPROPERTY()
	UXjAudioLoader* AudioLoader = nullptr;

private:

	void OnShowDebugChainVarChanged(IConsoleVariable* Var);
	
	void ShowDebugChain(const bool bShow);

	void UpdateDebugChainView();

	void RestoreRuntimeProjectDirectory(const FXjSettings& Settings);

	void DeleteRuntimeProjectDirectory();

	FString RuntimeProjectDir;

	TSharedPtr<SDebugChainView> DebugChainViewWidget;

	TMap<FString, FAudioPlayer> ActiveAudios;

	bool bMouseCursorInitialState = false;
};