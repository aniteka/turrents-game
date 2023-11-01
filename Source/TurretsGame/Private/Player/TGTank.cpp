// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTank.h"

ATGTank::ATGTank()
{
    PrimaryActorTick.bCanEverTick = true;

    Foundation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
    RootComponent = Foundation;

    Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
    Tower->SetupAttachment(Foundation);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
    Gun->SetupAttachment(Tower);
}

void ATGTank::BeginPlay()
{
    Super::BeginPlay();

}

void ATGTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATGTank::PrimaryAttack()
{
    Super::PrimaryAttack();

}
