// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ButtonBase.h"

#include "Managers/TurretWorldManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PCH/PCH_Interface.h"


// Sets default values
AButtonBase::AButtonBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Collider");
	RootComponent = BoxCollision;
	SetRootComponent(BoxCollision);

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void AButtonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AButtonBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AButtonBase::CheckpointOverlapped);
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AButtonBase::CheckpointOverlapped);
		BoxCollision->OnComponentEndOverlap.RemoveDynamic(this, &AButtonBase::CheckpointOverlapEnd);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AButtonBase::CheckpointOverlapEnd);
	}
}

void AButtonBase::Interact_Implementation()
{
	if (!bInteracted)
	{
		IInteractionInterface::Interact_Implementation();
		if (UWorld* World = GetWorld())
		{
			if (UTurretWorldManager* Manager = World->GetSubsystem<UTurretWorldManager>())
			{
				Manager->DisableAllTurrets();
				bInteracted = true;
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Turrets Disabled");
	}
}

void AButtonBase::CheckpointOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool UFromSweep, const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UPCH_Interface::StaticClass()))
	{
		IPCH_Interface::Execute_SetOverlappedActor(OtherActor, this);
	}
}

void AButtonBase::CheckpointOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UPCH_Interface::StaticClass()))
	{
		IPCH_Interface::Execute_SetOverlappedActor(OtherActor, nullptr);
	}
}

// Called every frame
void AButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

