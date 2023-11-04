// TurretGame by Team #1. AlphaNova courses

#include "UI/HUD/TG_HUD.h"
#include "UI/Widgets/TGMenuWidget.h"
#include "UI/Widgets/TGOverlayWidget.h"
#include "Components/ProgressBar.h"
#include "GameMode/TGGameMode.h"
#include "GameMode/TGMenuGameMode.h"
#include "Player/TGTank.h"
#include "Player/TGTurret.h"

void ATG_HUD::BeginPlay()
{
    Super::BeginPlay();

    if (!GetWorld()) return;

    const auto GameModeMenu = GetWorld()->GetAuthGameMode<ATGMenuGameMode>();
    if (GameModeMenu)
    {
        AddMenuWidget();
    }
    else
    {
        const auto GameMode = GetWorld()->GetAuthGameMode<ATGGameMode>();
        if (GameMode)
        {
            AddOverlayWidget();
            OverlayWidgetCreateHandle();
        }
    }
}

void ATG_HUD::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    UpdateShootDelayBar();
}

void ATG_HUD::AddMenuWidget()
{
    if (!GetOwningPlayerController()) return;

    MenuWidget = CreateWidget<UTGMenuWidget>(GetOwningPlayerController(), MenuWidgetClass);
    if (!MenuWidget) return;

    MenuWidget->AddToViewport();
}

void ATG_HUD::AddOverlayWidget()
{
    if (!GetOwningPlayerController()) return;

    OverlayWidget = CreateWidget<UTGOverlayWidget>(GetOwningPlayerController(), OverlayWidgetClass);
    if (!OverlayWidget) return;

    OverlayWidget->AddToViewport();
}

void ATG_HUD::OverlayWidgetCreateHandle()
{
    ATGTank* PlayerTank = Cast<ATGTank>(GetOwningPawn());
    if (PlayerTank)
    {
        EnableHealthBar();
        EnableShootDelayBar();
        EnableSpeedBar();

        SetPercentHealthBar(PlayerTank->GetHealthPercent());
    }
    else
    {
        ATGTurret* PlayerTurret = Cast<ATGTurret>(GetOwningPawn());
        if (!PlayerTurret) return;

        EnableHealthBar();
        EnableShootDelayBar();
    }
}

void ATG_HUD::UpdateShootDelayBar()
{
    OwnerPawn = (!OwnerPawn) ? Cast<ATGBasePawn>(GetOwningPawn()) : OwnerPawn;
    if (!OwnerPawn) return;

    SetPercentShootDelayBar(OwnerPawn->GetShootDelayPercent());
}

void ATG_HUD::EnableHealthBar()
{
    if (!OverlayWidget || !OverlayWidget->HealthBar) return;
    OverlayWidget->HealthBar->SetIsEnabled(true);
}

void ATG_HUD::EnableShootDelayBar()
{
    if (!OverlayWidget || !OverlayWidget->ShootDelayBar) return;
    OverlayWidget->ShootDelayBar->SetIsEnabled(true);
}

void ATG_HUD::EnableSpeedBar()
{
    if (!OverlayWidget || !OverlayWidget->SpeedBar) return;
    OverlayWidget->SpeedBar->SetIsEnabled(true);
}

void ATG_HUD::SetPercentHealthBar(float Percent)
{
    if (!OverlayWidget || !OverlayWidget->HealthBar || !OverlayWidget->HealthBar->GetIsEnabled()) return;
    OverlayWidget->HealthBar->SetPercent(Percent);
}

void ATG_HUD::SetPercentShootDelayBar(float Percent)
{
    if (!OverlayWidget || !OverlayWidget->ShootDelayBar || !OverlayWidget->ShootDelayBar->GetIsEnabled()) return;
    OverlayWidget->ShootDelayBar->SetPercent(Percent);
}

void ATG_HUD::SetPercentSpeedBar(float Percent)
{
    if (!OverlayWidget || !OverlayWidget->SpeedBar || !OverlayWidget->SpeedBar->GetIsEnabled()) return;
    OverlayWidget->SpeedBar->SetPercent(Percent);
}
