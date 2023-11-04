// TurretGame by Team #1. AlphaNova courses

#include "Player/TGPlayerController.h"
#include "UI/HUD/TG_HUD.h"

void ATGPlayerController::SetPercentHealthBar(float Percent) 
{
    UpdateHUDVar();
    if (!TG_HUD) return;

    TG_HUD->SetPercentHealthBar(Percent);
}

void ATGPlayerController::SetPercentShootDelayBar(float Percent) 
{
    UpdateHUDVar();
    if (!TG_HUD) return;

    TG_HUD->SetPercentShootDelayBar(Percent);
}

void ATGPlayerController::SetPercentSpeedBar(float Percent) 
{
    UpdateHUDVar();
    if (!TG_HUD) return;

    TG_HUD->SetPercentSpeedBar(Percent);
}

void ATGPlayerController::UpdateHUDVar()
{
    TG_HUD = (!TG_HUD) ? GetHUD<ATG_HUD>() : TG_HUD;
}
