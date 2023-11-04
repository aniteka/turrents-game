// TurretGame by Team #1. AlphaNova courses

#include "UI/Widgets/TGMenuWidget.h"
#include "Components/Button.h"
#include "Player/TGMenuPlayerController.h"

void UTGMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!PlayTankButton || !PlayTurretButton) return;

    PlayTankButton->OnClicked.AddDynamic(this, &UTGMenuWidget::OnPlayTankButtonClicked);
    PlayTurretButton->OnClicked.AddDynamic(this, &UTGMenuWidget::OnPlayTurretButtonClicked);
}

void UTGMenuWidget::OnPlayTankButtonClicked()
{
    StartPlayByGameType(EGameType::EGT_PlayTank);
}

void UTGMenuWidget::OnPlayTurretButtonClicked() 
{
    StartPlayByGameType(EGameType::EGT_PlayTurret);
}

void UTGMenuWidget::StartPlayByGameType(EGameType Type) 
{
    auto Controller = Cast<ATGMenuPlayerController>(GetOwningPlayer());
    if (!Controller) return;

    Controller->StartPlayByGameType(Type);
}
