// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TurretBase.generated.h"

class UArrowComponent;
class AProjectileBase;
class UNiagaraFunctionLibrary;

UCLASS()
class CPP_UNREAL_API ATurretBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurretBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BarrelMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> FirePoint;

	UPROPERTY(EditDefaultsOnly, Category = "TurretData")
	float FireSpeed = 0.5f;

	UPROPERTY(EditInstanceOnly, Category = "TurretData")
	FTimerHandle FireTimer;

	UPROPERTY(EditDefaultsOnly, Category = "TurretData")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "TurretData")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "TurretData")
	float FireAudioVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretData")
	TObjectPtr<UNiagaraSystem> FireFX;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TurretData")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "TurretData")
	void SetTurretEnabled(bool bEnabled);

private:
	bool bTurretEnabled = true;
	
};
