// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "IADataConfig.generated.h"

/**
 * 
 */
UCLASS()
class CPP_UNREAL_API UIADataConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> Look;
};
