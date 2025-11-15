// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCH_Interface.h"
#include "GameFramework/Character.h"
#include "CPP_Unreal/PCH/IA_Interface.h"
#include "PCH_Base.generated.h"

class UHealthComponent;
class USpringArmComponent;
class UCameraComponent;

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> Health;

	UPROPERTY(BlueprintReadWrite, Category = "Speeds")
	float DefaultSpeed = 400.0f;
	float BoostSpeed = 800.0f;
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
	virtual void Action_Implementation(const FInputActionInstance& Instance) override;

	virtual void SetOverlappedActor_Implementation(AActor* OverlappedActor) override;

	UFUNCTION()
	void PlayerDeath();
	
};
