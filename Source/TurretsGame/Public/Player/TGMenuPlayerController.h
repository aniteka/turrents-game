// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TurretGameTypes.h"
#include "TGMenuPlayerController.generated.h"

class ATG_HUD;

UCLASS()
class TURRETSGAME_API ATGMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void StartPlayByGameType(EGameType Type);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ATG_HUD* TG_HUD;

private:
    void UpdateHUDVar();
    void AddMenuWidget();
};
