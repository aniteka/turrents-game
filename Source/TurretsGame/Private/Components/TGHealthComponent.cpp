// TurretGame by Team #1. AlphaNova courses

#include "Components/TGHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TGPlayerController.h"

UTGHealthComponent::UTGHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTGHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner())
    {
        GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTGHealthComponent::OnTakeAnyDamageCallback);
    }
    SetHp(MaxHp);
}

void UTGHealthComponent::SetHp(float NewHp)
{
    if (!GetOwner()) return;

    const auto OldHp = Hp;
    Hp = FMath::Clamp(NewHp, 0.f, MaxHp);
    OnHpChangeDelegate.Broadcast(GetOwner(), Hp, Hp - OldHp);

    DeathCheck(Hp);
}

void UTGHealthComponent::AddDeltaToHp(float Delta)
{
    SetHp(GetHp() + Delta);
}

void UTGHealthComponent::DeathCheck(float InHp)
{
    if (!FMath::IsNearlyZero(Hp) || !GetWorld() || !GetOwner()) return;

    bIsDead = true;
    OnDeathDelegate.Broadcast(GetOwner());
}

float UTGHealthComponent::GetHealthPercent() const
{
    return Hp / MaxHp;
}

void UTGHealthComponent::OnTakeAnyDamageCallback(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    AddDeltaToHp(-Damage);
}
