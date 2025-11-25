// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleBase.h"

#include "CPP_Unreal/Enemies/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AObstacleBase::AObstacleBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionObjectType(ECC_WorldStatic);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollision->SetGenerateOverlapEvents(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxCollision);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Health = CreateDefaultSubobject<UHealthComponent>("Health");
}

// Called when the game starts or when spawned
void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();

	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AObstacleBase::OnObstacleOverlap);
	}
	
	if (Health)
	{
		/*
		BoxCollision->OnComponentHit.AddDynamic(this, &AObstacleBase::OnHit);
		*/
		Health->OnDeath.AddDynamic(this, &AObstacleBase::HandleDeath);
	}
}

// Called every frame
void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0, 100.0f * DeltaTime, 0));
}

void AObstacleBase::OnObstacleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (AProjectileBase* Projectile = Cast<AProjectileBase>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Obstacle hit by projectile: %s"), *OtherActor->GetName());
		UGameplayStatics::ApplyDamage(this, Projectile->BaseDamage, Projectile->GetInstigatorController(), Projectile, nullptr);

		if (DamageFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this, DamageFX, GetActorLocation(), GetActorRotation());
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this, HitSound, GetActorLocation(), HitAudioVolume);
		}
		
		Projectile->Destroy();
	}
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