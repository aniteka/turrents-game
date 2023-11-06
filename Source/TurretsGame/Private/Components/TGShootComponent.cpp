// TurretGame by Team #1. AlphaNova courses

#include "Components/TGShootComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
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

bool UTGShootComponent::CanShootNow() const
{
    return !IsShootDelay();
}

bool UTGShootComponent::IsShootDelay() const
{
    return GetWorld()->GetTimerManager().IsTimerActive(ShootDelayTimerHandle);
}

float UTGShootComponent::GetRemainsOfShootDelay() const
{
    if (!GetWorld()) return 0.f;

    if (GetWorld()->GetTimerManager().IsTimerActive(ShootDelayTimerHandle))
    {
        return GetWorld()->GetTimerManager().GetTimerRemaining(ShootDelayTimerHandle);
    }
    else
    {
        return ShootDelayInSec;
    }
}

void UTGShootComponent::DrawCrosshair(USceneComponent* Component, FName Socket, FVector ShootDirection)
{
    // TODO
}

void UTGShootComponent::ShootDelayCallback()
{
    GetWorld()->GetTimerManager().ClearTimer(ShootDelayTimerHandle);
    if (bUseDeferredShot && bShootImmediatelyAfterDelay)
    {
        bShootImmediatelyAfterDelay = false;
        ShootImplementation(AfterDelayInfo);
    }
}

bool UTGShootComponent::PreShootCheck(const FInfoForShoot& Info)
{
    if (!bUseShootDelay) return true;
    if (bShootImmediatelyAfterDelay) return false;

    if (IsShootDelay())
    {
        if (bUseDeferredShot && (RemainingTimeForDeferredShot == -1.f)
                ? true
                : GetWorld()->GetTimerManager().GetTimerRemaining(ShootDelayTimerHandle) <= RemainingTimeForDeferredShot)
        {
            bShootImmediatelyAfterDelay = true;
            AfterDelayInfo = Info;
        }
        return false;
    }

    if (!IsShootDelay())
    {
        GetWorld()->GetTimerManager().SetTimer(ShootDelayTimerHandle, this, &UTGShootComponent::ShootDelayCallback, ShootDelayInSec);
    }
    return true;
}

ATGProjectileBaseActor* UTGShootComponent::ShootImplementation(const UTGShootComponent::FInfoForShoot& Info)
{
    if (!PreShootCheck(Info)) return nullptr;

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = GetOwner<APawn>();
    SpawnParameters.CustomPreSpawnInitalization = [Speed = InitialProjectileSpeed](AActor* Actor)
    {
        Actor->GetComponentByClass<UProjectileMovementComponent>()->InitialSpeed = Speed;
    };

    const auto Projectile =
        GetWorld()->SpawnActor<ATGProjectileBaseActor>(ProjectileClass, Info.Location, Info.Direction.Rotation(), SpawnParameters);

    if (!IsValid(Projectile)) return nullptr;

    if (bProjectileSetLifeSpanAfterSpawn)
    {
        Projectile->SetLifeSpan(ProjectileLifeSpanInSec);
    }
    return Projectile;
}

float UTGShootComponent::GetShootDelayPercent() const
{
    return GetRemainsOfShootDelay() / ShootDelayInSec;
}
