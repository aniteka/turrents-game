// TurretGame by Team #1. AlphaNova courses


#include "Components/TGHealthComponent.h"

UTGHealthComponent::UTGHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

}

void UTGHealthComponent::PostInitProperties()
{
    Super::PostInitProperties();

    Hp = MaxHp;
}

void UTGHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTGHealthComponent::OnTakeAnyDamageCallback);
}

void UTGHealthComponent::SetHp(float NewHp)
{
    Hp = FMath::Clamp(NewHp, 0.f, MaxHp);
    DeathCheck(Hp);
}

void UTGHealthComponent::AddDeltaToHp(float Delta)
{
    SetHp(GetHp() + Delta);
}

void UTGHealthComponent::DeathCheck(float InHp)
{
    if (Hp != 0.f)
        return;

    bIsDead = true;
    OnDeathDelegate.Broadcast(GetOwner());
}

void UTGHealthComponent::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
    AController* InstigatedBy, AActor* DamageCauser)
{
    AddDeltaToHp(-Damage);
}
