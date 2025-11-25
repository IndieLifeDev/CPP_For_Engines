// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "CPP_Unreal/PCH/HealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

class UHealthComponent;

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 2.0f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetSphereRadius(64.0f);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->SetGenerateOverlapEvents(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
	StaticMeshComponent->SetCollisionProfileName("NoCollision");

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = SphereComponent;
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bSweepCollision = true;

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectileBase::OnProjectileOverlap);
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

void AProjectileBase::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}
	
	if (UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>())
	{
		// VFX??
		UE_LOG(LogTemp, Warning, TEXT("Projectile hit: %s"), *OtherActor->GetName());

		UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, nullptr, this, nullptr);

		if (HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitEffect,
				OtherActor->GetActorLocation(), GetActorRotation());
		}
		
		Destroy();
	}
}