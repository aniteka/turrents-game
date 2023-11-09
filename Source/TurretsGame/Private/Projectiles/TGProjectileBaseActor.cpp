// TurretGame by Team #1. AlphaNova courses

#include "Projectiles/TGProjectileBaseActor.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

ATGProjectileBaseActor::ATGProjectileBaseActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    SphereCollision->SetSimulatePhysics(true);
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereCollision->SetCollisionResponseToAllChannels(ECR_Block);
    SetRootComponent(SphereCollision);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
}

void ATGProjectileBaseActor::BeginPlay()
{
    Super::BeginPlay();

    SpawnTrailSystem();
}

void ATGProjectileBaseActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (SphereCollision)
    {
        SphereCollision->OnComponentHit.AddDynamic(this, &ATGProjectileBaseActor::StaticMeshComponentEventHitCallback);
    }
}

void ATGProjectileBaseActor::StaticMeshComponentEventHitCallback(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || GetInstigator() == OtherActor) return;

    UGameplayStatics::ApplyPointDamage(
        OtherActor, HitDamage, FVector::ZeroVector, Hit, GetInstigator()->GetController(), this, UDamageType::StaticClass());

    SpawnDestroyCosmetics();
    DeactivateAndDestroy();
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

void ATGProjectileBaseActor::SpawnTrailSystem()
{
    if (TrailSystem)
    {
        TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(  //
            TrailSystem,                                                //
            GetRootComponent(),                                         //
            FName(),                                                    //
            GetActorLocation(),                                         //
            GetActorRotation(),                                         //
            EAttachLocation::KeepWorldPosition,                         //
            false                                                       //
        );
    }
}

void ATGProjectileBaseActor::DeactivateAndDestroy()
{
    if (!SphereCollision || !StaticMeshComponent) return;

    SphereCollision->SetSimulatePhysics(false);
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetVisibility(false);
    SetLifeSpan(LifeSpanAfterHit);
}
