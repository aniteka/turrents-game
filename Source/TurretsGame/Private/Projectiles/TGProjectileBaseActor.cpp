// TurretGame by Team #1. AlphaNova courses

#include "Projectiles/TGProjectileBaseActor.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ATGProjectileBaseActor::ATGProjectileBaseActor()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMeshComponent->SetSimulatePhysics(true);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SetRootComponent(StaticMeshComponent);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
}

void ATGProjectileBaseActor::BeginPlay()
{
    Super::BeginPlay();

    check(GetInstigator());
}

void ATGProjectileBaseActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATGProjectileBaseActor::StaticMeshComponentEventHitCallback);
}

void ATGProjectileBaseActor::StaticMeshComponentEventHitCallback(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor) return;
    if (GetInstigator() == OtherActor) return;

    UGameplayStatics::ApplyPointDamage(
        OtherActor, HitDamage, FVector::ZeroVector, Hit, GetInstigator()->GetController(), this, UDamageType::StaticClass());

    this->Destroy();
}
