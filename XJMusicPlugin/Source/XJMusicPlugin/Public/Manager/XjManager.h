// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types/XjTypes.h"
#include "Engine/EngineBase.h"
#include "Manager/XjAudioLoader.h"
#include "../Settings/SettingsActor.h"
#include "XjManager.generated.h"

enum class XjCommandType
{
	TaxonomyChange,
	MacrosChange,
	IntensityChange
};

struct XjCommand
{
	XjCommandType Type;

	FString Arguments;

	float FloatValue = 0.0f;
};

class FXjRunnable : public FRunnable
{

public:

	FXjRunnable(UWorld* World);

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void PushCommand(const XjCommand& NewCommand)
	{
		Commands.Enqueue(NewCommand);
	}

	TimeRecord GetAtChainMicros() const
	{
		return AtChainMicros;
	}

	TWeakPtr<TEngineBase> GetActiveEngine() const
	{
		return Engine;
	}

private:

	class UXjMusicInstanceSubsystem* XjMusicSubsystem = nullptr;

	class UXjAudioLoader* AudioLoader = nullptr;

	TSharedPtr<TEngineBase> Engine;

	FXjSettings Settings;

	double LastFrameTime = 0.0f;

	int RunCycleFrequency = 9;

	TimeRecord XjStartTime;

	TimeRecord AtChainMicros;

	FThreadSafeBool bShouldStop = false;

	TQueue<XjCommand> Commands;
};

UCLASS()
class XJMUSICPLUGIN_API UXjManager : public UObject
{
	GENERATED_BODY()

public:

	void Setup();

	virtual void BeginDestroy() override;

	void PushCommand(const XjCommand& NewCommand)
	{
		if (!XjRunnable)
		{
			return;
		}

		XjRunnable->PushCommand(NewCommand);
	}

	TimeRecord GetAtChainMicros() const
	{
		if (!XjRunnable)
		{
			return {};
		}

		return XjRunnable->GetAtChainMicros();
	}

	TWeakPtr<TEngineBase> GetActiveEngine() const
	{
		if (!XjRunnable)
		{
			return {};
		}

		return XjRunnable->GetActiveEngine();
	}

private:

	TSharedPtr<FXjRunnable> XjRunnable;
	TSharedPtr<FRunnableThread> XjThread;
};