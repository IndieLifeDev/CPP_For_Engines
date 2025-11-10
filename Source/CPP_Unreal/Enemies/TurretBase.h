// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretBase.generated.h"

class UArrowComponent;
class AProjectileBase;

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TurretData")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "TurretData")
	void SetTurretEnabled(bool bEnabled);

private:
	bool bTurretEnabled = true;
	
};
