// Fill out your copyright notice in the Description page of Project Settings.


#include "TwinStickPawn.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	MyPC_Ref = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called every frame
void ATwinStickPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OldRotation = FRotator(GetActorRotation());
	if (bIsUsingMouse)
	{
		if (!MyPC_Ref)
		{
			return;
		}

		FHitResult Hit;
		if (MyPC_Ref->GetHitResultUnderCursorByChannel(TraceChannel, true, Hit))
		{
			const FVector HitLocation = Hit.Location;
			const FVector OwnerLocation = GetActorLocation();

			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, HitLocation);

			AimAngle = LookAt.Yaw;

			FRotator NewRotation = FRotator(OldRotation.Pitch, AimAngle, OldRotation.Roll);
			SetActorRotation(NewRotation);
		}
		else
		{
			FRotator NewRotation = FRotator(OldRotation.Pitch, AimAngle, OldRotation.Roll);
			SetActorRotation(NewRotation);
		}
	}

}

// Called to bind functionality to input
void ATwinStickPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATwinStickPawn::SetActorOverlapEvent_Implementation(AActor* OverlappedActor)
{
	ICPP_PlayerInterface::SetActorOverlapEvent_Implementation(OverlappedActor);
	MyOverlappedActorRef = OverlappedActor;
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
	bIsDashing = DashActive;

	if (DashActive)
	{
		// Dash logic
	}
	else
	{
		// slowwwwww downnnn
	}
}