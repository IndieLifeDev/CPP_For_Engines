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
		Projectile->Destroy();
	}
}


/*void AObstacleBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnHit Fired! OtherActor: %s"), *GetNameSafe(OtherActor));

	if (AProjectileBase* Projectile = Cast<AProjectileBase>(OtherActor))
	{
		// Uses projectiles base damage from its class
		float DamageAmount = Projectile->BaseDamage;
		
		UE_LOG(LogTemp, Warning, TEXT("Hit by projectile! Damage = %f"), DamageAmount);

		// Applies damage to the object as the Health Component receives it with OnTakeAnyDamage
		UGameplayStatics::ApplyDamage(this, DamageAmount,
			Projectile->GetInstigatorController(), Projectile, nullptr);
		
		// Destroys Projectile
		Projectile->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHit called, but OtherActor is NOT projectile."))
	}
}*/

/*void AObstacleBase::HandleDamageTaken(float DamageAmount)
{
	// FX
}*/

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
