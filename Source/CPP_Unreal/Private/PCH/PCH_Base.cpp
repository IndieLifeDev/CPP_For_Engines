// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Unreal/Public/PCH/PCH_Base.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CPP_Unreal/PCH/HealthComponent.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "CPP_Unreal/Enemies/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interactables/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
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

	LeftMuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePointLeft"));
	LeftMuzzlePoint->SetupAttachment(ShipMesh);
	LeftMuzzlePoint->SetRelativeLocation(FVector(200, 0, 0));

	RightMuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePointRight"));
	RightMuzzlePoint->SetupAttachment(ShipMesh);
	RightMuzzlePoint->SetRelativeLocation(FVector(-200, 0, 0));
	
	AirFX = CreateDefaultSubobject<UNiagaraComponent>("AirFX");
	AirFX->SetupAttachment(RootComponent);
	AirFX->bAutoActivate = false;
	
	BoostFX = CreateDefaultSubobject<UNiagaraComponent>("BoostFX");
	BoostFX->SetupAttachment(RootComponent);
	BoostFX->bAutoActivate = false;

	BoostAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BoostAudio"));
	BoostAudioComponent->SetupAttachment(RootComponent);
	BoostAudioComponent->bAutoActivate = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArm->bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void APCH_Base::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

	if (AirFX && AirFXSystem)
	{
		AirFX->SetAsset(AirFXSystem);
		AirFX->Activate();
	}

	if (FlyingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, FlyingSound, GetActorLocation(), FlyingAudioVolume);
	}

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

	// Handle accelerating and decelerating
	if (bAccelerating)
	{
		CurrentSpeed += AccelerationRate * DeltaTime;
	}

	if (bDecelerating)
	{
		CurrentSpeed -= DecelerationRate * DeltaTime;
	}

	// Clamp Speed
	CurrentSpeed = FMath::Clamp(CurrentSpeed, MinSpeed, MaxSpeed);

	// Apply Boost to Current Movement
	float DisplaySpeed = CurrentSpeed;
	if (bBoosting)
	{
		DisplaySpeed *= BoostMultiplier;
	}

	// Move Ship Forwards
	if (Controller)
	{
		GetCharacterMovement()->MaxWalkSpeed = DisplaySpeed;
		AddMovementInput(FVector::ForwardVector, 1.0f);
	}

	// Reset Ships Rotation when not Steering to normal
	if (GetWorld() && (GetWorld()->GetTimeSeconds() - LastSteerInputTime > SteerInputTimeout))
	{
		CurrentSteerInput = 0.0f;
	}

	// Ship Steering (Rotation of the Ship)
	float TargetRoll = FMath::Clamp(CurrentSteerInput * 30.0f, -30.0f, 30.0f);
	FRotator CurrentRotation = ShipMesh->GetRelativeRotation();
	FRotator TargetRotation = FRotator(TargetRoll, CurrentRotation.Yaw, CurrentRotation.Roll);
	ShipMesh->SetRelativeRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f));

	FRotator MuzzleRotation = LeftMuzzlePoint->GetRelativeRotation();
	FRotator MuzzleTargetRotation = FRotator(MuzzleRotation.Pitch, MuzzleRotation.Yaw, TargetRoll);
	LeftMuzzlePoint->SetRelativeRotation(FMath::RInterpTo(MuzzleRotation, MuzzleTargetRotation, DeltaTime, 5.0f));
	RightMuzzlePoint->SetRelativeRotation(FMath::RInterpTo(MuzzleRotation, MuzzleTargetRotation, DeltaTime, 5.0f));
}

// Called to bind functionality to input
void APCH_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Steer Action Implementation
void APCH_Base::SteerAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::SteerAction_Implementation(Instance);

	const FVector2D MoveValue = Instance.GetValue().Get<FVector2D>();
	
	if (FMath::Abs(MoveValue.X) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(FVector::RightVector, MoveValue.X);
		CurrentSteerInput = MoveValue.X;

		LastSteerInputTime = GetWorld()->GetTimeSeconds();
	}
}

// Accelerate Action Implementation
void APCH_Base::AccelerateAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::AccelerateAction_Implementation(Instance);

	bAccelerating = true;
}

// Accelerate Stop Action Implementation
void APCH_Base::AccelerateStopAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::AccelerateStopAction_Implementation(Instance);
	
	bAccelerating = false;
	UE_LOG(LogTemp, Warning, TEXT("ACCELERATE STOPPED FIRED"))
}

// Decelerate Action Implementation
void APCH_Base::DecelerateAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::DecelerateAction_Implementation(Instance);
	
	bDecelerating = true;
}

// Decelerate Stop Action Implementation
void APCH_Base::DecelerateStopAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::DecelerateStopAction_Implementation(Instance);
	
	bDecelerating = false;
	UE_LOG(LogTemp, Warning, TEXT("DECELERATE STOPPED FIRED"))
}

// Look Action Implementation
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

// Boost Action Implementation
void APCH_Base::BoostAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::BoostAction_Implementation(Instance);
	
	bBoosting = true;
	
	if (BoostFX && BoostFXSystem)
	{
		BoostFX->SetAsset(BoostFXSystem);
		BoostFX->Activate();
	}

	if (BoostAudioComponent && !BoostAudioComponent->IsPlaying())
	{
		BoostAudioComponent->Play();
	}
	
	Camera->FieldOfView = BoostFOV;
}

// Boost Stop Action Implementation
void APCH_Base::BoostStopAction_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::BoostAction_Implementation(Instance);

	bBoosting = false;

	if (BoostFX)
	{
		BoostFX->Deactivate();
	}

	if (BoostAudioComponent && BoostAudioComponent->IsPlaying())
	{
		BoostAudioComponent->FadeOut(0.25f, 0.0f);
	}
	
	Camera->FieldOfView = DefaultFOV;
}

// Action Implementation
void APCH_Base::Action_Implementation(const FInputActionInstance& Instance)
{
	IIA_Interface::Action_Implementation(Instance);
	//bool bValue = Instance.GetValue().Get<bool>();

	if (OverlappingActor)
	{
		IInteractionInterface::Execute_Interact(OverlappingActor);
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire");
}

// Start Firing Implementation
void APCH_Base::StartFiring_Implementation()
{
	if (!bIsFiring)
	{
		bIsFiring = true;
		FireProjectile();

		if (ShootSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this, ShootSound, GetActorLocation(), ShootingAudioVolume);
		}
		
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &APCH_Base::FireProjectile, FireRate, true);
	}
}

// Stop Firing Implementation
void APCH_Base::StopFiring_Implementation()
{
	bIsFiring = false;

	if (ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, ShootSound, GetActorLocation(), ShootingAudioVolume);
	}
	
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

// Set Overlapped Actor Action Implementation
void APCH_Base::SetOverlappedActor_Implementation(AActor* OverlappedActor)
{
	IPCH_Interface::SetOverlappedActor_Implementation(OverlappedActor);
	OverlappingActor = OverlappedActor;
}

// Fire Projectile
void APCH_Base::FireProjectile()
{
	if (!ProjectileClass || !LeftMuzzlePoint)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	const FVector LeftSpawnLocation = LeftMuzzlePoint->GetComponentLocation();
	const FRotator LeftSpawnRotation = LeftMuzzlePoint->GetComponentRotation();
	const FVector RightSpawnLocation = RightMuzzlePoint->GetComponentLocation();
	const FRotator RightSpawnRotation = RightMuzzlePoint->GetComponentRotation();

	AProjectileBase* LeftProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass,
		LeftSpawnLocation, LeftSpawnRotation, SpawnParameters);

	AProjectileBase* RightProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass,
		RightSpawnLocation, RightSpawnRotation, SpawnParameters);

	if (LeftProjectile && RightProjectile)
	{
		float ShipSpeed = GetVelocity().Size();
		float FinalSpeed = BaseProjectileSpeed + ShipSpeed;

		LeftProjectile->ProjectileMovementComponent->InitialSpeed = FinalSpeed;
		RightProjectile->ProjectileMovementComponent->InitialSpeed = FinalSpeed;

		LeftProjectile->ProjectileMovementComponent->MaxSpeed = FinalSpeed;
		RightProjectile->ProjectileMovementComponent->MaxSpeed = FinalSpeed;

		LeftProjectile->ProjectileMovementComponent->Velocity = LeftSpawnRotation.Vector() * FinalSpeed;
		RightProjectile->ProjectileMovementComponent->Velocity = RightSpawnRotation.Vector() * FinalSpeed;
	}
}

// Player Death
void APCH_Base::PlayerDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, "Player Died! Disabling Input!");
	APC_Base* PC = Cast<APC_Base>(GetController());
	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);
}