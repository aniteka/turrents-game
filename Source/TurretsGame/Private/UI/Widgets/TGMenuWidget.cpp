// TurretGame by Team #1. AlphaNova courses

#include "UI/Widgets/TGMenuWidget.h"
#include "Components/Button.h"
#include "Player/TGMenuPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UTGMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BindButtons();
}

void UTGMenuWidget::BindButtons()
{
    if (!PlayTankButton || !PlayTurretButton) return;

    PlayTankButton->OnClicked.AddDynamic(this, &UTGMenuWidget::OnPlayTankButtonClicked);
    PlayTurretButton->OnClicked.AddDynamic(this, &UTGMenuWidget::OnPlayTurretButtonClicked);
    PlayTankButton->OnHovered.AddDynamic(this, &UTGMenuWidget::OnPlayTankButtonHovered);
    PlayTurretButton->OnHovered.AddDynamic(this, &UTGMenuWidget::OnPlayTurretButtonHovered);
    ExitButton->OnClicked.AddDynamic(this, &UTGMenuWidget::OnExitButtonClicked);
    ExitButton->OnHovered.AddDynamic(this, &UTGMenuWidget::OnExitButtonHovered);
}

void UTGMenuWidget::OnPlayTankButtonClicked()
{
    StartPlayByGameType(EGameType::EGT_PlayTank);
}

void UTGMenuWidget::OnPlayTurretButtonClicked()
{
    StartPlayByGameType(EGameType::EGT_PlayTurret);
}

void UTGMenuWidget::OnExitButtonClicked() 
{
    if (!GetWorld() || !GetOwningPlayer()) return;
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UTGMenuWidget::OnPlayTankButtonHovered()
{
    PlayHoveredSound();
}

void UTGMenuWidget::OnPlayTurretButtonHovered()
{
    PlayHoveredSound();
}

void UTGMenuWidget::OnExitButtonHovered() 
{
    PlayHoveredSound();
}

void UTGMenuWidget::StartPlayByGameType(EGameType Type)
{
    auto Controller = Cast<ATGMenuPlayerController>(GetOwningPlayer());
    if (!Controller) return;

    Controller->StartPlayByGameType(Type);
}

void UTGMenuWidget::PlayHoveredSound()
{
    if (!HoveredSound) return;
    UGameplayStatics::PlaySound2D(this, HoveredSound);
}
