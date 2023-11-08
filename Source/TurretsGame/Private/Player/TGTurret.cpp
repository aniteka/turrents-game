// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTurret.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/TGGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATGTurret::ATGTurret()
{
    SetRootComponent(Tower);
    Gun->SetupAttachment(Tower);
    SpringArmComp->SetupAttachment(GetRootComponent());
    CameraComp->SetupAttachment(SpringArmComp);
}
