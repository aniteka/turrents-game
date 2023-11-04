// TurretGame by Team #1. AlphaNova courses

#include "UI/Widgets/TGOverlayWidget.h"
#include "Components/ProgressBar.h"

void UTGOverlayWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!HealthBar || !ShootDelayBar || !SpeedBar) return;

    HealthBar->SetIsEnabled(false);
    ShootDelayBar->SetIsEnabled(false);
    SpeedBar->SetIsEnabled(false);
}
