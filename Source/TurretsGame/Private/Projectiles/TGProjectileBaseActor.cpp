// TurretGame by Team #1. AlphaNova courses


#include "Projectiles/TGProjectileBaseActor.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"


ATGProjectileBaseActor::ATGProjectileBaseActor()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    SetRootComponent(StaticMeshComponent);
    StaticMeshComponent->SetSimulatePhysics(true);
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

void ATGProjectileBaseActor::StaticMeshComponentEventHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if(!OtherActor)
        return;   

    UGameplayStatics::ApplyPointDamage(OtherActor, HitDamage, FVector::ZeroVector, Hit,
        GetInstigator()->GetController(), this, UDamageType::StaticClass());

    this->Destroy();
}
