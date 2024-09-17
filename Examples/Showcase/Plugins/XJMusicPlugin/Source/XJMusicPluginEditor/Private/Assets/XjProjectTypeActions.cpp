// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.


#include "Assets/XjProjectTypeActions.h"
#include "Types/XjProject.h"

UClass* FXjProjectTypeActions::GetSupportedClass() const
{
	return UXjProject::StaticClass();
}

FText FXjProjectTypeActions::GetName() const
{
	return INVTEXT("XJ Project");
}

FColor FXjProjectTypeActions::GetTypeColor() const
{
	return FColor::Magenta;
}

uint32 FXjProjectTypeActions::GetCategories()
{
	return EAssetTypeCategories::Sounds;
}
