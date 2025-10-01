// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TwinStickController.generated.h"
//#include "EnhancedInputSubsystems.generated.h"

/**
 * 
 */
UCLASS()
class CPP_UNREAL_API ATwinStickController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* PossessedPawn) override;

	virtual void SetupInputComponent() override;

	/*UInputMappingContext* MappingContext;*/
};
