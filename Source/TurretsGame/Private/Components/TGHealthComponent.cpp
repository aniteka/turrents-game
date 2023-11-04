// TurretGame by Team #1. AlphaNova courses

#include "Components/TGHealthComponent.h"
#include "GameMode/TGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

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

    if (GetOwner())
    {
        GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTGHealthComponent::OnTakeAnyDamageCallback);
    }
}

void UTGHealthComponent::SetHp(float NewHp)
{
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

    auto GameMode = Cast<ATGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GameMode) return;

    auto AIController = GetOwner()->GetInstigatorController<AAIController>();
    if (AIController)
    {
        GameMode->EnemyDestroyed(GetOwner());
        GetOwner()->Destroy();
    }
    else
    {
        auto Controller = GetOwner()->GetInstigatorController();
        if (!Controller) return;

        GameMode->GameOver();
    }
}

void UTGHealthComponent::OnTakeAnyDamageCallback(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    AddDeltaToHp(-Damage);
}
