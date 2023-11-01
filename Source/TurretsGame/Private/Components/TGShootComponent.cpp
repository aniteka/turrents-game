// TurretGame by Team #1. AlphaNova courses

#include "Components/TGShootComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Projectiles/TGProjectileBaseActor.h"

UTGShootComponent::UTGShootComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

ATGProjectileBaseActor* UTGShootComponent::ShootFromLocation(FVector Location, FVector ShootDirection)
{
    return ShootImplementation({Location, ShootDirection});
}

ATGProjectileBaseActor* UTGShootComponent::ShootFromActor(AActor* Actor, FName Socket, FVector ShootDirection)
{
    check(Actor);
    FVector Location;
    FVector Direction;

    if (const auto MeshComponent = Actor->GetComponentByClass<UStaticMeshComponent>(); Socket != NAME_None && MeshComponent)
    {
        const auto Transform = MeshComponent->GetSocketTransform(Socket);
        Location = Transform.GetLocation();
        Direction = UKismetMathLibrary::GetForwardVector(Transform.Rotator());
    }
    else
    {
        Location = Actor->GetActorLocation();
        Direction = Actor->GetActorForwardVector();
    }

    if (ShootDirection != FVector::ZeroVector) Direction = ShootDirection;

    return ShootImplementation({Location, Direction});
}

ATGProjectileBaseActor* UTGShootComponent::ShootFromComponent(USceneComponent* Component, FName Socket, FVector ShootDirection)
{
    check(Component);
    FVector Location;
    FVector Direction;

    if (const auto MeshComponent = Cast<UStaticMeshComponent>(Component); Socket != NAME_None && MeshComponent)
    {
        const auto Transform = MeshComponent->GetSocketTransform(Socket);
        Location = Transform.GetLocation();
        Direction = UKismetMathLibrary::GetForwardVector(Transform.Rotator());
    }
    else
    {
        Location = Component->GetComponentLocation();
        Direction = Component->GetForwardVector();
    }

    if (ShootDirection != FVector::ZeroVector) Direction = ShootDirection;

    return ShootImplementation({Location, Direction});
}

bool UTGShootComponent::IsShootDelay() const
{
    return GetWorld()->GetTimerManager().IsTimerActive(ShootDelayTimerHandle);
}

float UTGShootComponent::GetRemainsOfShootDelay() const
{
    return GetWorld()->GetTimerManager().GetTimerRemaining(ShootDelayTimerHandle);
}

void UTGShootComponent::ShootDelayCallback()
{
    GetWorld()->GetTimerManager().ClearTimer(ShootDelayTimerHandle);
    if (bShootImmediatelyAfterDelay)
    {
        bShootImmediatelyAfterDelay = false;
        ShootImplementation(AfterDelayInfo);
    }
}

bool UTGShootComponent::ShootDelayCheck(const UTGShootComponent::FInfoForShoot& Info)
{
    if (bShootImmediatelyAfterDelay) return true;
    if (!bUseShootDelay) return false;

    if (GetWorld()->GetTimerManager().IsTimerActive(ShootDelayTimerHandle))
    {
        bShootImmediatelyAfterDelay = true;
        AfterDelayInfo = Info;
        return true;
    }
    GetWorld()->GetTimerManager().SetTimer(ShootDelayTimerHandle, this, &UTGShootComponent::ShootDelayCallback, ShootDelayInSec);
    return false;
}

ATGProjectileBaseActor* UTGShootComponent::ShootImplementation(const UTGShootComponent::FInfoForShoot& Info)
{
    if (ShootDelayCheck(Info)) return nullptr;

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = GetOwner<APawn>();

    const auto Projectile =
        GetWorld()->SpawnActor<ATGProjectileBaseActor>(ProjectileClass, Info.Location, Info.Direction.Rotation(), SpawnParameters);

    if (!IsValid(Projectile)) return nullptr;

    if (bProjectileSetLifeSpanAfterSpawn)
    {
        Projectile->SetLifeSpan(ProjectileLifeSpanInSec);
    }

    Projectile->GetStaticMeshComponent()->AddImpulse(Info.Direction * ProjectileImpulseMultiplier, ProjectileSocketToApplyImpulse);
    return Projectile;
}
