// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TwinStickPawn.generated.h"

UCLASS()
class CPP_UNREAL_API ATwinStickPawn : public APawn, public ICPP_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATwinStickPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DoMove(FVector2D Input);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateMouse(bool MouseActive);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateDash(bool DashActive);
	
public:
	FVector LastMoveInput = FVector(0.0f, 0.0f, 0.0f);

	bool UsingMouse = false;

	bool UsingDash = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetActorOverlapEvent_Implementation(AActor* OverlappedActor) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<AActor> MyOverlappedActorRef;

};
