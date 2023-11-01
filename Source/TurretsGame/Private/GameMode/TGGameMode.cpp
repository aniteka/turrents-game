// TurretGame by Team #1. AlphaNova courses

#include "GameMode/TGGameMode.h"
#include "Player/TGTank.h"
#include "Player/TGPlayerController.h"

ATGGameMode::ATGGameMode() 
{
    DefaultPawnClass = ATGTank::StaticClass();
    PlayerControllerClass = ATGPlayerController::StaticClass();
}

void ATGGameMode::BeginPlay()
{
    Super::BeginPlay();

    switch (GameType)
    {
        case EGameType::EGT_PlayTank: DefaultPawnClass = ATGTank::StaticClass();
            break;
        case EGameType::EGT_PlayTurret: // DefaultPawnClass = ATGTurret::StaticClass(); 
            break;
    }

    RestartPlayerAtTransform(GetTGPlayerController(), PlayerStartTransform);
}

ATGPlayerController* ATGGameMode::GetTGPlayerController()
{
    return (!TGPlayerController) ? Cast<ATGPlayerController>(GetWorld()->GetFirstPlayerController()) : TGPlayerController;
}
