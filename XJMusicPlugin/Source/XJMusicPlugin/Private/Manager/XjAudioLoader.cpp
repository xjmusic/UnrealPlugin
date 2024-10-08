// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.


#include "Manager/XjAudioLoader.h"
#include "Settings/XJMusicDefaultSettings.h"
#include "Misc/FileHelper.h"
#include "Sound/SoundWave.h"
#include "Engine/AssetManager.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "AudioDevice.h"
#include "Mixer/XjMixer.h"
#include "Manager/XjManager.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "XJMusicPlugin/Public/XjMusicInstanceSubsystem.h"

FXjAudioWave::FXjAudioWave(const FXjAudioWave& Other)
{
	Wave = Other.Wave;
	SamplesData = Other.SamplesData;
	NumSamples = Other.NumSamples;
}

const FXjAudioWave& FXjAudioWave::operator=(const FXjAudioWave& Other)
{
	Wave = Other.Wave;
	SamplesData = Other.SamplesData;
	NumSamples = Other.NumSamples;

	return *this;
}

void FXjAudioWave::LoadData(USoundWave* NewWave, const bool bAllowDecompression)
{
	if (!NewWave || NewWave == Wave)
	{
		return;
	}

	Wave = NewWave;

	if (Wave->RawPCMData == nullptr)
	{
		if (!bAllowDecompression)
		{
			return;
		}

		FAudioDevice* AudioDevice = GEngine->GetMainAudioDeviceRaw();
		if (!AudioDevice)
		{
			return;
		}

		EDecompressionType DecompressionType = Wave->DecompressionType;
		Wave->DecompressionType = DTYPE_Native;

		FName Format;
		
#if ENGINE_MAJOR_VERSION >= 5
		Format = Wave->GetRuntimeFormat();
#else
		Format = AudioDevice->GetRuntimeFormat(Wave);
#endif

		Bulk = Wave->GetCompressedData(Format, Wave->GetPlatformCompressionOverridesForCurrentPlatform());

		if (!Bulk)
		{
			return;
		}

		Wave->InitAudioResource(*Bulk);

		if (Wave->DecompressionType == DTYPE_RealTime || Wave->CachedRealtimeFirstBuffer != nullptr)
		{
			return;
		}

		FAsyncAudioDecompress DecompressTask(Wave, 128, AudioDevice);
		DecompressTask.StartSynchronousTask();

		Wave->DecompressionType = DTYPE_Native;

		FinishLoad();
	}
	else
	{
		FinishLoad();
	}
}

bool FXjAudioWave::IsValidToUse() const
{
	return Wave && SamplesData;
}

void FXjAudioWave::FinishLoad()
{
	TArrayView<uint8> Arr((uint8*)Wave->RawPCMData, Wave->RawPCMDataSize / 2);

	SamplesData = (int16*)Arr.GetData();
	NumSamples = Wave->RawPCMDataSize / sizeof(int16);
}

void UXjAudioLoader::Setup()
{
	UXJMusicDefaultSettings* XjSettings = GetMutableDefault<UXJMusicDefaultSettings>();
	if (!XjSettings)
	{
		return;
	}

	RetrieveProjectsContent();
}

void UXjAudioLoader::Shutdown()
{
	AudiosSoftReferences.Reset();

	CachedAudios.Reset();
}

void UXjAudioLoader::DecompressAll()
{
	for (const TPair<FString, TSoftObjectPtr<UObject>>& Info : AudiosSoftReferences)
	{
		GetSoundById(Info.Key, true);
	}
}

TSharedPtr<FXjAudioWave> UXjAudioLoader::GetSoundById(const FString& Id, const bool bAllowDecompression)
{
	if (TSharedPtr<FXjAudioWave>* Wave = CachedAudios.Find(Id))
	{
		return *Wave;
	}

	FSoftObjectPath* AudioPath  = AudiosSoftReferences.Find(Id);
	if (!AudioPath)
	{
		return {};
	}

	USoundWave* Wave = Cast<USoundWave>(AudioPath->ResolveObject());
	if (!Wave)
	{
		return {};
	}

	Wave->LoadingBehavior = ESoundWaveLoadingBehavior::ForceInline;

	TSharedPtr<FXjAudioWave> XjAudio = MakeShared<FXjAudioWave>();
	XjAudio->LoadData(Wave, bAllowDecompression);
	
	CachedAudios.Add(Id, XjAudio);

	return MoveTemp(XjAudio);
}

void UXjAudioLoader::RetrieveProjectsContent()
{
	FXjSettings Settings = GetSettings(GetWorld());
   
	if (!Settings.LaunchProject.IsValid())
	{
		return;
	}

    UXjProject* LoadedProject = Cast<UXjProject>(Settings.LaunchProject.ResolveObject());
    check(LoadedProject);

	for (TSoftObjectPtr<UObject> Obj : LoadedProject->AudiosReferences)
	{
		AudiosSoftReferences.Add(Obj.GetAssetName() + ".wav", Obj.ToSoftObjectPath());
	}

	TArray<FSoftObjectPath> Paths;
	AudiosSoftReferences.GenerateValueArray(Paths);
	InitialAssetsStream = StreamableManager.RequestAsyncLoad(Paths);

	if (!InitialAssetsStream->BindCompleteDelegate(FStreamableDelegate::CreateUObject(this, &UXjAudioLoader::OnAssetsLoaded)))
	{
		OnAssetsLoaded();
	}
}

void UXjAudioLoader::OnAssetsLoaded()
{
#if ENGINE_MAJOR_VERSION >= 5

	DecompressAll();

	bLoading.Store(false);

#else

	Async(EAsyncExecution::Thread, [this]()
		{
			DecompressAll();

			bLoading.Store(false);
		});

#endif

}
