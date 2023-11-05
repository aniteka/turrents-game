// TurretGame by Team #1. AlphaNova courses


#include "AI/Components/TG_AIShootComponent.h"

#include "Projectiles/TGProjectileBaseActor.h"


UTG_AIShootComponent::UTG_AIShootComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    bUseShootDelay = true;
}

ATGProjectileBaseActor* UTG_AIShootComponent::ShootImplementation(const FInfoForShoot& Info)
{
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = GetOwner<APawn>();

    const auto Projectile =
        GetWorld()->SpawnActor<ATGProjectileBaseActor>(ProjectileClass, Info.Location, Info.Direction.Rotation(), SpawnParameters);

    if (!IsValid(Projectile)) return nullptr;

    if (bProjectileSetLifeSpanAfterSpawn)
    {
        Projectile->SetLifeSpan(ProjectileLifeSpanInSec);
    }
    return Projectile;
}

