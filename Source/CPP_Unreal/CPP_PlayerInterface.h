// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCPP_PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CPP_UNREAL_API ICPP_PlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetActorOverlapEvent(AActor* OverlappedActor);
	
};
