// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TwinStickPawn.generated.h"

class UFloatingPawnMovement;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS()
class CPP_UNREAL_API ATwinStickPawn : public APawn, public ICPP_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATwinStickPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFloatingPawnMovement> Movement;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetActorOverlapEvent_Implementation(AActor* OverlappedActor) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<AActor> MyOverlappedActorRef;

	UPROPERTY()
	TObjectPtr<APlayerController> MyPC_Ref = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aim")
	TEnumAsByte<ETraceTypeQuery> TraceChannel = TraceTypeQuery1;

private:
	FVector LastMoveInput = FVector(0.0f, 0.0f, 0.0f);

	float AimAngle = 0.0f;

	FRotator OldRotation;
	
	bool bIsUsingMouse = false; // Will be set and/or unset in a different function

	bool bIsDashing = false;

};
