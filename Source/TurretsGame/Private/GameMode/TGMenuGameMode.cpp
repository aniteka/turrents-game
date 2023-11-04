// TurretGame by Team #1. AlphaNova courses

#include "GameMode/TGMenuGameMode.h"
#include "Player/TGMenuPlayerController.h"
#include "UI/HUD/TG_HUD.h"
#include "GameInstance/TGGameInstance.h"
#include "Kismet/GameplayStatics.h"

ATGMenuGameMode::ATGMenuGameMode() 
{
    PlayerControllerClass = ATGMenuPlayerController::StaticClass();
    HUDClass = ATG_HUD::StaticClass();
}

void ATGMenuGameMode::StartPlayByGameType(EGameType Type) 
{
    auto GameInstance = GetGameInstance<UTGGameInstance>();
    if (!GameInstance) return;

    GameInstance->ChoosedGameType = Type;
    UGameplayStatics::OpenLevel(this, GameInstance->BattleMapName);
}
