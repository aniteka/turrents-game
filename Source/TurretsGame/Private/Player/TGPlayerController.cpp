// TurretGame by Team #1. AlphaNova courses

#include "Player/TGPlayerController.h"

#include "AI/TG_AITeams.h"
#include "UI/HUD/TG_HUD.h"
#include "UI/Widgets/TGOverlayWidget.h"

void ATGPlayerController::SetPercentHealthBar(float Percent) 
{
    UpdateHUDVar();
    if (!TG_HUD) return;

    TG_HUD->SetPercentBar(Percent, TG_HUD->OverlayWidget->HealthBarImage);
}

void ATGPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    SetGenericTeamId(TGTeamId::PlayerId);
}

void ATGPlayerController::UpdateHUDVar()
{
    TG_HUD = (!TG_HUD) ? GetHUD<ATG_HUD>() : TG_HUD;
}
