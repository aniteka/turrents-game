// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TurretGameTypes.h"
#include "TGGameInstance.generated.h"

UCLASS()
class TURRETSGAME_API UTGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Game properties")
    FName BattleMapName = "MainMap";

    UPROPERTY(EditDefaultsOnly, Category = "TG|Game properties")
    FName MenuMapName = "MenuMap";

    EGameType ChoosedGameType = EGameType::EGT_PlayTank;
};
