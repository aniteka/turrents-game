// TurretGame by Team #1. AlphaNova courses

#include "UI/HUD/TG_HUD.h"
#include "UI/Widgets/TGMenuWidget.h"
#include "UI/Widgets/TGOverlayWidget.h"
#include "Components/Image.h"
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

    if (UpdateOwnerPawnVar() && OverlayWidget)
    {
        SetPercentBar(OwnerPawn->GetShootDelayPercent(), OverlayWidget->ShootDelayBarImage);
        SetPercentBar(OwnerPawn->GetSpeedPercent(), OverlayWidget->SpeedBarImage);
    }
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
        EnableBar(OverlayWidget->HealthBarImage, true);
        EnableBar(OverlayWidget->ShootDelayBarImage, true);
        EnableBar(OverlayWidget->SpeedBarImage, true);

        SetPercentBar(PlayerTank->GetHealthPercent(), OverlayWidget->HealthBarImage);
    }
    else
    {
        ATGTurret* PlayerTurret = Cast<ATGTurret>(GetOwningPawn());
        if (!PlayerTurret) return;

        EnableBar(OverlayWidget->HealthBarImage, true);
        EnableBar(OverlayWidget->ShootDelayBarImage, true);
    }
}

void ATG_HUD::EnableBar(UImage* BarToEnable, bool bEnable)
{
    if (!OverlayWidget || !BarToEnable) return;
    (bEnable) ? BarToEnable->SetVisibility(ESlateVisibility::Visible) : BarToEnable->SetVisibility(ESlateVisibility::Hidden);
}

void ATG_HUD::SetPercentBar(float Percent, UImage* BarToChange)
{
    if (!OverlayWidget || !BarToChange || BarToChange->GetVisibility() != ESlateVisibility::Visible) return;

    auto DymMaterial = BarToChange->GetDynamicMaterial();
    if (!DymMaterial) return;

    DymMaterial->SetScalarParameterValue(FName(TEXT("ProgressAlpha")), Percent);
}

bool ATG_HUD::UpdateOwnerPawnVar()
{
    OwnerPawn = (!OwnerPawn) ? Cast<ATGBasePawn>(GetOwningPawn()) : OwnerPawn;
    return OwnerPawn ? true : false;
}
