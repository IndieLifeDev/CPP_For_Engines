// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/TurretWorldManager.h"

#include "CPP_Unreal/Enemies/TurretBase.h"

void UTurretWorldManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Display, TEXT("Turret Manager Init"));
}

void UTurretWorldManager::Deinitialize()
{
	RegisteredTurrets.Empty();
	Super::Deinitialize();
	UE_LOG(LogTemp, Log, TEXT("Turret Manager Destroyed"));
}

void UTurretWorldManager::RegisterTurret(ATurretBase* Turret)
{
	if (Turret && !RegisteredTurrets.Contains(Turret))
	{
		RegisteredTurrets.Add(Turret);
		UE_LOG(LogTemp, Log, TEXT("Turret registered: %s"), *Turret->GetName());
	}
}

void UTurretWorldManager::UnregisterTurret(ATurretBase* Turret)
{
	if (Turret)
	{
		RegisteredTurrets.Remove(Turret);
		UE_LOG(LogTemp, Log, TEXT("Turret unregistered: %s"), *Turret->GetName());
	}
}

void UTurretWorldManager::DisableAllTurrets()
{
	for (auto& TurretWeakPtr : RegisteredTurrets)
	{
		if (ATurretBase* Turret = TurretWeakPtr.Get())
		{
			Turret->SetTurretEnabled(false);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("All turrets disabled"));
}