// TurretGame by Team #1. AlphaNova courses

#include "Projectiles/TGProjectileBaseActor.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"

ATGProjectileBaseActor::ATGProjectileBaseActor()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMeshComponent->SetSimulatePhysics(true);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SetRootComponent(StaticMeshComponent);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
}

void ATGProjectileBaseActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (StaticMeshComponent)
    {
        StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATGProjectileBaseActor::StaticMeshComponentEventHitCallback);
    }
}

void ATGProjectileBaseActor::StaticMeshComponentEventHitCallback(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    SpawnDestroyCosmetics();

    if (!OtherActor || GetInstigator() == OtherActor) return;

    UGameplayStatics::ApplyPointDamage(
        OtherActor, HitDamage, FVector::ZeroVector, Hit, GetInstigator()->GetController(), this, UDamageType::StaticClass());

    this->Destroy();
}

void ATGProjectileBaseActor::SpawnDestroyCosmetics() 
{
    if (ExplosionSound && ExplosionSystem && SmokeExplSystem)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this, ExplosionSystem, GetActorLocation(), GetActorRotation(), FVector(ExplosionScale));
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this, SmokeExplSystem, GetActorLocation(), GetActorRotation(), FVector(ExplosionScale));
    }
}
