// TurretGame by Team #1. AlphaNova courses

#include "Player/TGMenuPlayerController.h"
#include "UI/HUD/TG_HUD.h"
#include "GameMode/TGMenuGameMode.h"

void ATGMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AddMenuWidget();
}


void ATGMenuPlayerController::StartPlayByGameType(EGameType Type)
{
    if (!GetWorld()) return;

    auto GameMode = GetWorld()->GetAuthGameMode<ATGMenuGameMode>();
    if (!GameMode) return;

    GameMode->StartPlayByGameType(Type);
}

void ATGMenuPlayerController::AddMenuWidget()
{
    UpdateHUDVar();
    TG_HUD->AddMenuWidget();
}

void ATGMenuPlayerController::UpdateHUDVar() 
{
    TG_HUD = (!TG_HUD) ? GetHUD<ATG_HUD>() : TG_HUD;
}
