// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FXjProjectTypeActions : public FAssetTypeActions_Base
{	
public:
	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};
