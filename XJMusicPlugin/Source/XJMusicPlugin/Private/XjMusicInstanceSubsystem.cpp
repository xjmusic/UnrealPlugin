// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

#include "XjMusicInstanceSubsystem.h"
#include "Settings/XJMusicDefaultSettings.h"
#include "Async/Async.h"
#include "Manager/XjManager.h"
#include "Widgets/SWeakWidget.h"
#include "Sound/SoundConcurrency.h"
#include "Mixer/XjMixer.h"
#include "Manager/XjAudioLoader.h"

static TAutoConsoleVariable<int32> CVarShowDebugChain(
	TEXT("xj.showdebug"), 
	1, 
	TEXT("Show debug view of the chain schedule"), 
	ECVF_Default);

void UXjMusicInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CVarShowDebugChain->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UXjMusicInstanceSubsystem::OnShowDebugChainVarChanged));
}

void UXjMusicInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	DeleteRuntimeProjectDirectory();
}

void UXjMusicInstanceSubsystem::DoOverrideTaxonomy(const FString Taxonomy)
{
	if (!Manager)
	{
		return;
	}

	XjCommand Command;
	Command.Type = XjCommandType::TaxonomyChange;
	Command.Arguments = Taxonomy;

	Manager->PushCommand(Command);
}

void UXjMusicInstanceSubsystem::DoOverrideMacro(const FString Macro)
{
	if (!Manager)
	{
		return;
	}

	XjCommand Command;
	Command.Type = XjCommandType::MacrosChange;
	Command.Arguments = Macro;

	Manager->PushCommand(Command);
}

void UXjMusicInstanceSubsystem::DoOverrideIntensity(const float Value)
{
	if (!Manager)
	{
		return;
	}

	XjCommand Command;
	Command.Type = XjCommandType::IntensityChange;
	Command.FloatValue = Value;

	Manager->PushCommand(Command);
}

TArray<FString> UXjMusicInstanceSubsystem::GetAllTaxonomyMemes() const
{
	if (!Manager)
	{
		return {};
	}

	TSharedPtr<TEngineBase> Engine = Manager->GetActiveEngine().Pin();

	return Engine->GetAllTaxonomyMemes();
}

TArray<FString> UXjMusicInstanceSubsystem::GetAllMacros() const
{
	if (!Manager)
	{
		return {};
	}

	TSharedPtr<TEngineBase> Engine = Manager->GetActiveEngine().Pin();

	return Engine->GetAllMacros();
}

void UXjMusicInstanceSubsystem::SetupXJ()
{
	UXJMusicDefaultSettings* XjSettings = GetMutableDefault<UXJMusicDefaultSettings>();

	if(!XjSettings || !XjSettings->LaunchProject.IsValid())
	{
		return;
	}
	
	RestoreRuntimeProjectDirectory(XjSettings);
	
	if (IsValid(Manager))
	{
		return;
	}

	AudioLoader = NewObject<UXjAudioLoader>(this);
	if (AudioLoader)
	{
		AudioLoader->Setup();
	}

	Mixer = NewObject<UXjMixer>(this);
	if (Mixer)
	{
		Mixer->Setup(XjSettings->bDefaultOutput);
	}

	Manager = NewObject<UXjManager>(this);
	if (Manager)
	{
		Manager->Setup();
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		bMouseCursorInitialState = PlayerController->ShouldShowMouseCursor();
	}

	ShowDebugChain(XjSettings->bShowDebugTimeline);
}

void UXjMusicInstanceSubsystem::ShutdownXJ()
{
	DebugChainViewWidget.Reset();

	ActiveAudios.Empty();

	if (IsValid(Mixer))
	{
		Mixer->Shutdown();
	}

	if (IsValid(AudioLoader))
	{
		AudioLoader->Shutdown();
	}

	Manager = nullptr;

	Mixer = nullptr;

	AudioLoader = nullptr;

	DeleteRuntimeProjectDirectory();
}

void UXjMusicInstanceSubsystem::AddOrUpdateActiveAudio(const FAudioPlayer& Audio)
{
	if (!IsInGameThread())
	{
		AsyncTask(ENamedThreads::GameThread, [this, Audio]()
			{
				AddOrUpdateActiveAudio(Audio);
			});

		return;
	}

	if (ActiveAudios.Contains(Audio.Id))
	{
		ActiveAudios[Audio.Id] = Audio;
	}
	else
	{
		ActiveAudios.Add(Audio.Id, Audio);
	}

	UpdateDebugChainView();

	if (!Mixer)
	{
		return;
	}

	Mixer->AddOrUpdateActiveAudio(FMixerAudio::CreateMixerAudio(Audio, AudioLoader));
}

void UXjMusicInstanceSubsystem::RemoveActiveAudio(const FAudioPlayer& Audio)
{
	if (!IsInGameThread())
	{
		AsyncTask(ENamedThreads::GameThread, [this, Audio]()
			{
				RemoveActiveAudio(Audio);
			});

		return;
	}

	if (!ActiveAudios.Contains(Audio.Id))
	{
		return;
	}

	ActiveAudios.Remove(Audio.Id);

	UpdateDebugChainView();

	if (!Mixer)
	{
		return;
	}

	Mixer->RemoveActiveAudio(Audio.Id);
}

void UXjMusicInstanceSubsystem::OnShowDebugChainVarChanged(IConsoleVariable* Var)
{
	if (!Var)
	{
		return;
	}

	int Value = Var->GetInt();

	ShowDebugChain(Value > 0);
}

void UXjMusicInstanceSubsystem::ShowDebugChain(const bool bShow)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
	{
		return;
	}

	if (!bShow)
	{
		if (DebugChainViewWidget)
		{
			DebugChainViewWidget->SetVisibility(EVisibility::Hidden);
		}

		PlayerController->SetShowMouseCursor(bMouseCursorInitialState);
	}
	else
	{
		bMouseCursorInitialState = PlayerController->ShouldShowMouseCursor();
		PlayerController->SetShowMouseCursor(true);

		if (DebugChainViewWidget)
		{
			DebugChainViewWidget->SetVisibility(EVisibility::Visible);
		}
		else
		{
			if (!GEngine || !Manager)
			{
				return;
			}

			DebugChainViewWidget = SNew(SDebugChainView).Engine(Manager->GetActiveEngine());

			GEngine->GameViewport->AddViewportWidgetContent(
				SNew(SWeakWidget).PossiblyNullContent(DebugChainViewWidget.ToSharedRef()));

			DebugChainViewWidget->SetVisibility(EVisibility::Visible);
		}

		UpdateDebugChainView();
	}
}

void UXjMusicInstanceSubsystem::UpdateDebugChainView()
{
	if (!IsInGameThread())
	{
		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				UpdateDebugChainView();
			});

		return;
	}

	if (!Manager)
	{
		return;
	}

	if (DebugChainViewWidget)
	{
		ActiveAudios.ValueSort([](const FAudioPlayer& A, const FAudioPlayer& B)
			{
				return A.StartTime.GetMicros() < B.StartTime.GetMicros();
			});

		DebugChainViewWidget->UpdateActiveAudios(ActiveAudios, Manager->GetAtChainMicros());
	}
}

void UXjMusicInstanceSubsystem::RestoreRuntimeProjectDirectory(UXJMusicDefaultSettings* XjSettings)
{
	if(!XjSettings->LaunchProject.IsValid())
	{
		return;
	}

	XjProjectInstance = Cast<UXjProject>(XjSettings->LaunchProject.TryLoad());
	check(XjProjectInstance)

	DeleteRuntimeProjectDirectory();
	
	RuntimeProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + "TempXJ";
	
	XjProjectInstance->RestoreDirectory(RuntimeProjectDir);
}

void UXjMusicInstanceSubsystem::DeleteRuntimeProjectDirectory()
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if(!RuntimeProjectDir.IsEmpty() && PlatformFile.DirectoryExists(*RuntimeProjectDir))
	{
		PlatformFile.DeleteDirectoryRecursively(*RuntimeProjectDir);
	}
}
