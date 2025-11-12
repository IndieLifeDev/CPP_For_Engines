// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Unreal/Public/PCH/PCH_Base.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CPP_Unreal/PCH/HealthComponent.h"
#include "PCH/PC_Base.h"


// Sets default values
APCH_Base::APCH_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	Health = CreateDefaultSubobject<UHealthComponent>("Health");

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void APCH_Base::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	if (Health)
	{
		Health->OnDeath.AddUniqueDynamic(this, &APCH_Base::PlayerDeath);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, "No Health valid Component added...");
	}
	
}

// Called every frame
void APCH_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APCH_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void APCH_Base::MoveAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::MoveAction_Implementation(Instance);

	if (Controller)
	{
		const FVector2D MoveValue = Instance.GetValue().Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		if (FMath::Abs(MoveValue.Y) > KINDA_SMALL_NUMBER)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(Direction, MoveValue.Y);
		}
		if (FMath::Abs(MoveValue.X) > KINDA_SMALL_NUMBER)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void APCH_Base::LookAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::LookAction_Implementation(Instance);

	if (Controller)
	{
		const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();
		/*if (AxisValue.Y != 0.0f)
		{
			AddControllerPitchInput(-AxisValue.Y); // Inverts Y Input
		}*/
		if (AxisValue.X != 0.0f)
		{
			AddControllerYawInput(AxisValue.X);
		}
	}
}

void APCH_Base::JumpAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::JumpAction_Implementation(Instance);
	Jump();
}

void APCH_Base::Action_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::Action_Implementation(Instance);
	bool bValue = Instance.GetValue().Get<bool>();

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire");
}

void APCH_Base::PlayerDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, "Player Died! Disabling Input!");
	APC_Base* PC = Cast<APC_Base>(GetController());
	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);
}

