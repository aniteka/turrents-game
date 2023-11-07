// TurretGame by Team #1. AlphaNova courses

#include "Player/TGMenuPlayerController.h"
#include "UI/HUD/TG_HUD.h"
#include "GameMode/TGMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ATGMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputModeUIOnly();
    PlayMusic();
}

void ATGMenuPlayerController::StartPlayByGameType(EGameType Type)
{
    if (!GetWorld()) return;

    auto GameMode = GetWorld()->GetAuthGameMode<ATGMenuGameMode>();
    if (!GameMode) return;

    GameMode->StartPlayByGameType(Type);
}

void ATGMenuPlayerController::SetInputModeUIOnly()
{
    FInputModeUIOnly InputModeUI;
    SetInputMode(InputModeUI);
    SetShowMouseCursor(true);
}

void ATGMenuPlayerController::PlayMusic()
{
    if (!MenuTheme) return;
    UGameplayStatics::PlaySound2D(this, MenuTheme);
}
