// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTurret.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/TGGameMode.h"

ATGTurret::ATGTurret()
{
    Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
    SetRootComponent(Tower);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
    Gun->SetupAttachment(Tower);
}

void ATGTurret::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    OnTakePointDamage.AddDynamic(this, &ATGTurret::OnTakePointDamageCallback);
}

void ATGTurret::OnTakePointDamageCallback(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
    if (!GetWorld()) return;

    auto GameMode = Cast<ATGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GameMode) return;

    GameMode->EnemyDestroyed(this);
    Destroy();
}
