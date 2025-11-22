// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"

#include "PC_Base.generated.h"

class UIADataConfig;

UCLASS()
class CPP_UNREAL_API APC_Base : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY()
	TObjectPtr<AActor> LocalPCH;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UIADataConfig> InputActions;

	void Steer(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void Action(const FInputActionInstance& Instance);
	void Accelerate(const FInputActionInstance& Instance);
	void AccelerateStopped(const FInputActionInstance& Instance);
	void Decelerate(const FInputActionInstance& Instance);
	void DecelerateStopped(const FInputActionInstance& Instance);
	void Boost(const FInputActionInstance& Instance);
	void BoostStopped(const FInputActionInstance& Instance);
};
