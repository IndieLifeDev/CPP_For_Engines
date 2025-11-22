// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Unreal/Public/PCH/PC_Base.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CPP_Unreal/PCH/IA_Interface.h"
#include "CPP_Unreal/Public/PCH/Inputs/IADataConfig.h"

void APC_Base::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (MappingContext)
		{
			Subsystem->ClearAllMappings();
			Subsystem ->AddMappingContext(MappingContext, 0);
			
		}
	}

	LocalPCH = GetPawn();
}

void APC_Base::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent))
	{
		PEI->BindAction(InputActions->Steer.LoadSynchronous(), ETriggerEvent::Triggered, this, &APC_Base::Steer);
		PEI->BindAction(InputActions->Look.LoadSynchronous(), ETriggerEvent::Triggered, this, &APC_Base::Look);
		PEI->BindAction(InputActions->Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APC_Base::Action);
		/*PEI->BindAction(InputActions->Boost.LoadSynchronous(), ETriggerEvent::Triggered, this, &APC_Base::Boost);*/

		// Accelerate / Decelerate Actions
		PEI->BindAction(InputActions->Accelerate.LoadSynchronous(), ETriggerEvent::Triggered, this, &APC_Base::Accelerate);
		PEI->BindAction(InputActions->Accelerate.LoadSynchronous(), ETriggerEvent::Completed, this, &APC_Base::AccelerateStopped);
		PEI->BindAction(InputActions->Decelerate.LoadSynchronous(), ETriggerEvent::Triggered, this, &APC_Base::Decelerate);
		PEI->BindAction(InputActions->Decelerate.LoadSynchronous(), ETriggerEvent::Completed, this, &APC_Base::DecelerateStopped);

		// Boost Action
		PEI->BindAction(InputActions->Boost.LoadSynchronous(), ETriggerEvent::Triggered, this, &APC_Base::Boost);
		PEI->BindAction(InputActions->Boost.LoadSynchronous(), ETriggerEvent::Completed, this, &APC_Base::BoostStopped);
		
	}
}

void APC_Base::Steer(const FInputActionInstance& Instance)
{
	IIA_Interface::Execute_SteerAction(LocalPCH, Instance);
}

void APC_Base::Look(const FInputActionInstance& Instance)
{
	IIA_Interface::Execute_LookAction(LocalPCH, Instance);
}

void APC_Base::Action(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Player did an Action"));
	IIA_Interface::Execute_Action(LocalPCH, Instance);
}

void APC_Base::Accelerate(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Player ACCELERATING"));
	IIA_Interface::Execute_AccelerateAction(LocalPCH, Instance);
}

void APC_Base::AccelerateStopped(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Player ACCELERATING STOPPED"));
	IIA_Interface::Execute_AccelerateStopAction(LocalPCH, Instance);
}

void APC_Base::Decelerate(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Player DECELERATING"));
	IIA_Interface::Execute_DecelerateAction(LocalPCH, Instance);
}
void APC_Base::DecelerateStopped(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Player DECELERATING STOPPED"));
	IIA_Interface::Execute_DecelerateStopAction(LocalPCH, Instance);
}

void APC_Base::Boost(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Boost TRIGGERED"));
	IIA_Interface::Execute_BoostAction(LocalPCH, Instance);
}

void APC_Base::BoostStopped(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Boost RELEASED"));
	IIA_Interface::Execute_BoostStopAction(LocalPCH, Instance);
}
