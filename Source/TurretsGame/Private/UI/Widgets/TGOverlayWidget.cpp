// TurretGame by Team #1. AlphaNova courses

#include "UI/Widgets/TGOverlayWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UTGOverlayWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!HealthBarImage || !ShootDelayBarImage || !SpeedBarImage || !EnemyHealthBarImage) return;

    HealthBarImage->SetVisibility(ESlateVisibility::Hidden);
    ShootDelayBarImage->SetVisibility(ESlateVisibility::Hidden);
    SpeedBarImage->SetVisibility(ESlateVisibility::Hidden);
    EnemyHealthBarImage->SetVisibility(ESlateVisibility::Hidden);
}
