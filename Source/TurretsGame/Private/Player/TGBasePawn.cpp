// TurretGame by Team #1. AlphaNova courses


#include "Player/TGBasePawn.h"

#include "Components/TGShootComponent.h"

ATGBasePawn::ATGBasePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    ShootComp = CreateDefaultSubobject<UTGShootComponent>(TEXT("ShootComp"));
}
