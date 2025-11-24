// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PCH_Interface.h"
#include "CPP_Unreal/Enemies/ProjectileBase.h"
#include "GameFramework/Character.h"
#include "CPP_Unreal/PCH/IA_Interface.h"
#include "PCH_Base.generated.h"

class UHealthComponent;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;

UCLASS()
class CPP_UNREAL_API APCH_Base : public ACharacter, public IIA_Interface, public IPCH_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APCH_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	UStaticMeshComponent* ShipMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* AirFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* AirFXSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* BoostFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* BoostFXSystem;


	// Current Ship Speed
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CurrentSpeed = 1000.0f;

	// Minimum and Maximum Speeds
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinSpeed = 800.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 1500.0f;

	// Acceleration and Deceleration Rates
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AccelerationRate = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DecelerationRate = 400.0f;

	// Boost
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BoostMultiplier = 1.5f;
	bool bBoosting = false;

	// Inputs
	bool bAccelerating = false;
	bool bDecelerating = false;

	// FOV
	int DefaultFOV = 110;
	int BoostFOV = 130;

	// Steering
	float CurrentSteerInput = 0.0f;
	float LastSteerInputTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Steering")
	float SteerInputTimeout = 0.08f;

	UPROPERTY(BlueprintReadOnly, Category = "Interactions")
	TObjectPtr<AActor> OverlappingActor;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.1f;

	FTimerHandle FireTimer;

	bool bIsFiring = false;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USceneComponent* LeftMuzzlePoint;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USceneComponent* RightMuzzlePoint;

	float BaseProjectileSpeed = 10000.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SteerAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void LookAction_Implementation(const FInputActionInstance& Instance) override;

	virtual void AccelerateAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void AccelerateStopAction_Implementation(const FInputActionInstance& Instance) override;

	virtual void DecelerateAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void DecelerateStopAction_Implementation(const FInputActionInstance& Instance) override;
	
	virtual void BoostAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void BoostStopAction_Implementation(const FInputActionInstance& Instance) override;

	virtual void Action_Implementation(const FInputActionInstance& Instance) override;

	virtual void SetOverlappedActor_Implementation(AActor* OverlappedActor) override;

	UFUNCTION(BlueprintNativeEvent)
	void StartFiring();
	
	UFUNCTION(BlueprintNativeEvent)
	void StopFiring();

	UFUNCTION()
	void FireProjectile();
	
	UFUNCTION()
	void PlayerDeath();
	
};
