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
    const auto Projectile = GetWorld()->SpawnActor<ATGProjectileBaseActor>(ProjectileClass, Location, ShootDirection.Rotation());
    if (!IsValid(Projectile))
        return nullptr;
    Projectile->GetStaticMeshComponent()->AddImpulse(ShootDirection * ProjectileImpulseMultiplier, ProjectileSocketToApplyImpulse);
    if (bProjectileSetLifeSpanAfterSpawn)
        Projectile->SetLifeSpan(ProjectileLifeSpanInSec);
    return Projectile;
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

    if (ShootDirection != FVector::ZeroVector)
        Direction = ShootDirection;

    return ShootFromLocation(Location, Direction);
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

    if (ShootDirection != FVector::ZeroVector)
        Direction = ShootDirection;

    return ShootFromLocation(Location, Direction);
}
