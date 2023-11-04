// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TurretGameTypes.h"
#include "TGMenuGameMode.generated.h"

UCLASS()
class TURRETSGAME_API ATGMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATGMenuGameMode();

    void StartPlayByGameType(EGameType Type);
};
