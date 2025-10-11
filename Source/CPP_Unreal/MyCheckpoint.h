// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Includes
#include "CoreMinimal.h"
#include "CPP_InteractionMessages.h"
#include "GameFramework/Actor.h"
#include "MyCheckpoint.generated.h"

// Forward Declarations
class UBoxComponent;

// Delgate declares
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndGameSignature, FString, EndMessage);

UCLASS()
class CPP_UNREAL_API AMyCheckpoint : public AActor, public ICPP_InteractionMessages
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyCheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void DoInteract_Implementation() override;

	UPROPERTY(BlueprintAssignable)
	FEndGameSignature OnEndGameDelegate;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

private:
	UFUNCTION()
	void CheckpointOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void CheckpointOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
