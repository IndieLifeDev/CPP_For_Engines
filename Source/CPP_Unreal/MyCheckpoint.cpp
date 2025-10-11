// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCheckpoint.h"

#include "CPP_PlayerInterface.h"
#include "TwinStickGamemode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMyCheckpoint::AMyCheckpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Collider");
	RootComponent = BoxCollision;
	SetRootComponent(BoxCollision);
	
}

// Called when the game starts or when spawned
void AMyCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	ATwinStickGamemode* GM = Cast<ATwinStickGamemode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->RegisterCheckpoint(this);
	}
	
}

void AMyCheckpoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AMyCheckpoint::CheckpointOverlapped);
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyCheckpoint::CheckpointOverlapped);
		BoxCollision->OnComponentEndOverlap.RemoveDynamic(this, &AMyCheckpoint::CheckpointOverlapEnd);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMyCheckpoint::CheckpointOverlapEnd);
	}
}

// Called every frame
void AMyCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCheckpoint::DoInteract_Implementation()
{
	ICPP_InteractionMessages::DoInteract_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("I've been interacted"));
	if (OnEndGameDelegate.IsBound())
	{
		OnEndGameDelegate.Broadcast(TEXT("EndGame"));
	}
}

void AMyCheckpoint::CheckpointOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped!"));

	if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UCPP_PlayerInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap actor has interface"));
		ICPP_PlayerInterface::Execute_SetActorOverlapEvent(OtherActor, this);

	}
}

void AMyCheckpoint::CheckpointOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Error, TEXT("Overlapped Ended!"));

	if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UCPP_PlayerInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap actor has interface"));
		ICPP_PlayerInterface::Execute_SetActorOverlapEvent(OtherActor, nullptr);

	}
}