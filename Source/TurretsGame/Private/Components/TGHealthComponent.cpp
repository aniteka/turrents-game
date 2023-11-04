// TurretGame by Team #1. AlphaNova courses

#include "Components/TGHealthComponent.h"
#include "GameMode/TGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
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

    UpdateHUD();
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

void UTGHealthComponent::UpdateHUD()
{
    if (!GetOwner()) return;

    auto Pawn = Cast<APawn>(GetOwner());
    if (!Pawn) return;

    TGPlayerController = (!TGPlayerController) ? Pawn->GetController<ATGPlayerController>() : TGPlayerController;
    if (!TGPlayerController) return;

    TGPlayerController->SetPercentHealthBar(GetHealthPercent());
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
