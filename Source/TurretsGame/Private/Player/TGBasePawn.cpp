// TurretGame by Team #1. AlphaNova courses

#include "Player/TGBasePawn.h"
#include "Components/TGShootComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATGBasePawn::ATGBasePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    ShootComp = CreateDefaultSubobject<UTGShootComponent>(TEXT("ShootComp"));
}

void ATGBasePawn::ChangeTowerRotator()
{
    FRotator TowerRot = Tower->GetRelativeRotation();
    TowerRot.Yaw = SpringArmComp->GetTargetRotation().Yaw;

    Tower->SetRelativeRotation(TowerRot, true);
}
