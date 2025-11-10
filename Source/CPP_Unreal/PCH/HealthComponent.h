// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEvent);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPP_UNREAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnDamaged(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float CurrentHealth = 0.0f;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Health)
	FOnDeathEvent OnDeath;

	

	
};
