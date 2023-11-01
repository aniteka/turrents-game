// TurretGame by Team #1. AlphaNova courses


#include "Projectiles/TGProjectileBaseActor.h"


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
}
