// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TurretWorldManager.generated.h"

class ATurretBase;
/**
 * 
 */
UCLASS()
class CPP_UNREAL_API UTurretWorldManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Called when subsystem is initialized
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	// Called when subsystem is deinitialized
	virtual void Deinitialize() override;

	// Register and unregister turrets
	void RegisterTurret(ATurretBase* TurretActor);
	void UnregisterTurret(ATurretBase* TurretActor);

	// Disable all registered turrets
	UFUNCTION(BlueprintCallable)
	void DisableAllTurrets();

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<ATurretBase>> RegisteredTurrets;
	// Introduce weak pointers prevents dangling pointers if a turret is destroyed
	// and covers protection against garbage collection
};
