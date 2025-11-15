// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Unreal/Public/PCH/PCH_Base.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CPP_Unreal/PCH/HealthComponent.h"
#include "NiagaraComponent.h"
#include "Interactables/InteractionInterface.h"
#include "PCH/PC_Base.h"


// Sets default values
APCH_Base::APCH_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	Health = CreateDefaultSubobject<UHealthComponent>("Health");

	BoostFX = CreateDefaultSubobject<UNiagaraComponent>("BoostFX");
	BoostFX->SetupAttachment(RootComponent);
	BoostFX->bAutoActivate = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void APCH_Base::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

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

	if (Controller)
	{
		float TargetRoll = FMath::Clamp(CurrentSteerInput * 30.0f, -30.0f, 30.0f);

		FRotator CurrentRotation = ShipMesh->GetRelativeRotation();
		FRotator TargetRotation = FRotator(TargetRoll, CurrentRotation.Yaw, CurrentRotation.Roll);

		ShipMesh->SetRelativeRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f));
	}
}

// Called to bind functionality to input
void APCH_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APCH_Base::MoveAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::MoveAction_Implementation(Instance);

	const FVector2D MoveValue = Instance.GetValue().Get<FVector2D>();
		
	if (FMath::Abs(MoveValue.Y) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(FVector::ForwardVector, MoveValue.Y);
	}
	if (FMath::Abs(MoveValue.X) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(FVector::RightVector, MoveValue.X);
	}

	CurrentSteerInput = MoveValue.X;
}

void APCH_Base::LookAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::LookAction_Implementation(Instance);

	if (Controller)
	{
		const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();
		
		if (AxisValue.X != 0.0f)
		{
			AddControllerYawInput(AxisValue.X);
		}
	}
}

void APCH_Base::BoostAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::BoostAction_Implementation(Instance);
	
	GetCharacterMovement()->MaxWalkSpeed = BoostSpeed;
	if (BoostFX && BoostFXSystem)
	{
		Camera->FieldOfView = BoostFOV;
		BoostFX->SetAsset(BoostFXSystem);
		BoostFX->Activate();
	}
}

void APCH_Base::BoostStopAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::BoostAction_Implementation(Instance);

	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	if (BoostFX)
	{
		Camera->FieldOfView = DefaultFOV;
		BoostFX->Deactivate();
	}
}

void APCH_Base::Action_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::Action_Implementation(Instance);
	bool bValue = Instance.GetValue().Get<bool>();

	if (OverlappingActor)
	{
		IInteractionInterface::Execute_Interact(OverlappingActor);
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire");
}


void APCH_Base::SetOverlappedActor_Implementation(AActor* OverlappedActor)
{
	IPCH_Interface::SetOverlappedActor_Implementation(OverlappedActor);
	OverlappingActor = OverlappedActor;
}


void APCH_Base::PlayerDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, "Player Died! Disabling Input!");
	APC_Base* PC = Cast<APC_Base>(GetController());
	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);
}