// TurretGame by Team #1. AlphaNova courses

#include "Player/TGPlayerController.h"
#include "AI/TG_AITeams.h"
#include "UI/HUD/TG_HUD.h"
#include "UI/Widgets/TGOverlayWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ATGPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    SetGenericTeamId(TGTeamId::PlayerId);
    SetInputModeGameOnly();
    PlayMusic();
}

void ATGPlayerController::EnableEnemyHealthBar(bool bEnable)
{
    UpdateHUDVar();
    if (!TG_HUD) return;

    TG_HUD->EnableBar(TG_HUD->OverlayWidget->EnemyHealthBarImage, bEnable);
}

void ATGPlayerController::SetPercentHealthBar(float Percent)
{
    UpdateHUDVar();
    if (!TG_HUD || !TG_HUD->OverlayWidget || !TG_HUD->OverlayWidget->HealthBarImage) return;

    SetPercentBar(Percent, TG_HUD->OverlayWidget->HealthBarImage);
}

void ATGPlayerController::SetPercentEnemyHealthBar(float Percent)
{
    UpdateHUDVar();
    if (!TG_HUD || !TG_HUD->OverlayWidget || !TG_HUD->OverlayWidget->EnemyHealthBarImage) return;

    SetPercentBar(Percent, TG_HUD->OverlayWidget->EnemyHealthBarImage);
}

void ATGPlayerController::SetPercentBar(float Percent, UImage* BarToChange)
{
    UpdateHUDVar();
    if (!TG_HUD || !BarToChange) return;

    TG_HUD->SetPercentBar(Percent, BarToChange);
}

void ATGPlayerController::UpdateHUDVar()
{
    TG_HUD = (!TG_HUD) ? GetHUD<ATG_HUD>() : TG_HUD;
}

void ATGPlayerController::SetInputModeGameOnly()
{
    FInputModeGameOnly InputModeGameOnly;
    SetInputMode(InputModeGameOnly);
    SetShowMouseCursor(false);
}

void ATGPlayerController::PlayMusic()
{
    if (!BattleTheme) return;
    UGameplayStatics::PlaySound2D(this, BattleTheme);
}
