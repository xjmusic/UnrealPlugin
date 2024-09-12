// Copyright (c) XJ Music Inc. (https://xjmusic.com) All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "XjProjectTypeFactory.generated.h"

UCLASS()
class XJMUSICPLUGINEDITOR_API UXjProjectTypeFactory : public UFactory
{
	GENERATED_BODY()

public:
	UXjProjectTypeFactory();
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
