// Fill out your copyright notice in the Description page of Project Settings.


#include "TwinStickController.h"

void ATwinStickController::BeginPlay()
{
	Super::BeginPlay();
}

void ATwinStickController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwinStickController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
}

void ATwinStickController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}
