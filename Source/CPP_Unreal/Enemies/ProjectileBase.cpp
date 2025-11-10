// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 2.0f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetSphereRadius(64.0f);
	SphereComponent->SetCollisionProfileName("BlockAllDynamic");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
	StaticMeshComponent->SetCollisionProfileName("NoCollision");

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	SphereComponent->OnComponentHit.AddUniqueDynamic(this, &AProjectileBase::ProjectileHit);

	
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, nullptr, this, nullptr);
	// VFX??
	Destroy();
}

