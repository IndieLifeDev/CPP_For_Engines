// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleBase.h"

#include "NiagaraFunctionLibrary.h"
#include "CPP_Unreal/Enemies/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AObstacleBase::AObstacleBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	Health = CreateDefaultSubobject<UHealthComponent>("Health");
}

// Called when the game starts or when spawned
void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();

	if (Health)
	{
		MeshComponent->OnComponentHit.AddDynamic(this, &AObstacleBase::OnHit);
		Health->OnDeath.AddDynamic(this, &AObstacleBase::HandleDeath);
	}
}

// Called every frame
void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0, 100.0f * DeltaTime, 0));
}

void AObstacleBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (AProjectileBase* Projectile = Cast<AProjectileBase>(OtherActor))
	{
		// Uses projectiles base damage from its class
		float DamageAmount = Projectile->BaseDamage;

		// Applies damage to the object as the Health Component receives it with OnTakeAnyDamage
		UGameplayStatics::ApplyDamage(this, DamageAmount,
			Projectile->GetInstigatorController(), Projectile, nullptr);

		// Destroys Projectile
		Projectile->Destroy();
	}
}

void AObstacleBase::HandleDamageTaken(float DamageAmount)
{
	// FX
}

void AObstacleBase::HandleDeath()
{
	if (DestroyFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this, DestroyFX, GetActorLocation(), GetActorRotation());
	}

	if (DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, DestroySound, GetActorLocation(), DestroyAudioVolume);
	}

	Destroy();
}
