// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Rootable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URootable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STRAW_API IRootable
{
	GENERATED_BODY()

public:
	virtual void Root() = 0;
};
