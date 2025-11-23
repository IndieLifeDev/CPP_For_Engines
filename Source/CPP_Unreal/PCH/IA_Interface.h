// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputAction.h"
#include "IA_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIA_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CPP_UNREAL_API IIA_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Action(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SteerAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LookAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BoostAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BoostStopAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AccelerateAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AccelerateStopAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DecelerateAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DecelerateStopAction(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintNativeEvent)
	void StartFiring();

	UFUNCTION(BlueprintNativeEvent)
	void StopFiring();
};
