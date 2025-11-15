// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PCH_Interface.h"
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
	UNiagaraComponent* BoostFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* BoostFXSystem;
	
	UPROPERTY(BlueprintReadWrite, Category = "Speeds")
	float DefaultSpeed = 1000.0f;
	float BoostSpeed = 2500.0f;
	int DefaultFOV = 110;
	int BoostFOV = 130;

	float CurrentSteerInput = 0.0f;
	//float BoostAcceleration = 800.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Interactions")
	TObjectPtr<AActor> OverlappingActor;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void LookAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void BoostAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void BoostStopAction_Implementation(const FInputActionInstance& Instance) override;
	virtual void Action_Implementation(const FInputActionInstance& Instance) override;

	virtual void SetOverlappedActor_Implementation(AActor* OverlappedActor) override;

	UFUNCTION()
	void PlayerDeath();
	
};
