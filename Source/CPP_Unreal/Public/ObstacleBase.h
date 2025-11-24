// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CPP_Unreal/PCH/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "ObstacleBase.generated.h"

class UNiagaraSystem;

UCLASS()
class CPP_UNREAL_API AObstacleBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstacleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> Health;

	UPROPERTY(EditAnywhere, Category = "Audio")
	TObjectPtr<USoundBase> DestroySound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	float DestroyAudioVolume = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<UNiagaraSystem> DestroyFX;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleDamageTaken(float DamageAmount);
};
