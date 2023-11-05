// TurretGame by Team #1. AlphaNova courses

#include "Player/TGMenuPlayerController.h"
#include "UI/HUD/TG_HUD.h"
#include "GameMode/TGMenuGameMode.h"

void ATGMenuPlayerController::StartPlayByGameType(EGameType Type)
{
    if (!GetWorld()) return;

    auto GameMode = GetWorld()->GetAuthGameMode<ATGMenuGameMode>();
    if (!GameMode) return;

    GameMode->StartPlayByGameType(Type);
}
