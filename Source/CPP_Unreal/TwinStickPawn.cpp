// Fill out your copyright notice in the Description page of Project Settings.


#include "TwinStickPawn.h"

// Sets default values
ATwinStickPawn::ATwinStickPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATwinStickPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATwinStickPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATwinStickPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATwinStickPawn::ActivateMouse_Implementation(bool MouseActive)
{
	//UsingMouse = MouseActive;

	if (MouseActive)
	{
		// Receive Input
	}
	else
	{
		// do a flip or something
	}
}

void ATwinStickPawn::DoMove_Implementation(FVector2D Input)
{
	LastMoveInput.X = Input.X;
	LastMoveInput.Y = Input.Y;

	FRotator MoveDirection = FRotator(0.0f, GetControlRotation().Yaw, GetControlRotation().Roll);

	GetMovementComponent()->AddInputVector(MoveDirection.RotateVector(FVector::ForwardVector) * Input.X);
	GetMovementComponent()->AddInputVector(MoveDirection.RotateVector(FVector::RightVector) * Input.Y);
}

void ATwinStickPawn::ActivateDash_Implementation(bool DashActive)
{
	UsingDash = DashActive;

	if (DashActive)
	{
		// Dash logic
	}
	else
	{
		// slowwwwww downnnn
	}
}