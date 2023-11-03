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

void ATGBasePawn::ChangeGunRotator()
{
    FRotator GunRot = Gun->GetRelativeRotation();
    GunRot.Pitch = FMath::ClampAngle(SpringArmComp->GetTargetRotation().Pitch, -GunPitchThreshold, GunPitchThreshold);

    Gun->SetRelativeRotation(GunRot, true);
}
