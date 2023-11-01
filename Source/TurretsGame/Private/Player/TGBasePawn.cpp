// TurretGame by Team #1. AlphaNova courses


#include "Player/TGBasePawn.h"

#include "Components/TGShootComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ATGBasePawn::ATGBasePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    ShootComp = CreateDefaultSubobject<UTGShootComponent>(TEXT("ShootComp"));
    MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
}
