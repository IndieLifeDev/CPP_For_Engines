// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBase.h"
#include "ProjectileBase.h"
#include "Components/ArrowComponent.h"


// Sets default values
ATurretBase::ATurretBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	SetRootComponent(BaseMeshComponent);

	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMeshComponent"));
	BarrelMeshComponent->SetupAttachment(BaseMeshComponent);
	BarrelMeshComponent->SetRelativeLocation(FVector(60.0f, 0.0f, 0.0f));
	BarrelMeshComponent->SetRelativeScale3D(FVector(0.5f));

	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(BarrelMeshComponent);
	FirePoint->SetRelativeScale3D(FVector(0.5f));
}

// Called when the game starts or when spawned
void ATurretBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FireTimer, this, &ATurretBase::Fire_Implementation, FireSpeed, true);
	
}

// Called every frame
void ATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurretBase::Fire_Implementation()
{
	const FVector Location = FirePoint->GetComponentLocation();
	const FRotator Rotation = FirePoint->GetComponentRotation();

	GetWorld()->SpawnActor(ProjectileClass, &Location, &Rotation);
}

void ATurretBase::SetTurretEnabled(bool bEnabled)
{
	bTurretEnabled = bEnabled;
	if (!bTurretEnabled)
	{
		GetWorldTimerManager().ClearTimer(FireTimer);
	}
	else
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &ATurretBase::Fire_Implementation, FireSpeed, true);
	}

}

