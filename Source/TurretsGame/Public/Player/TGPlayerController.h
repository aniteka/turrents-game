// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TGPlayerController.generated.h"

class ATG_HUD;

UCLASS()
class TURRETSGAME_API ATGPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void SetPercentHealthBar(float Percent);
    void SetPercentShootDelayBar(float Percent);
    void SetPercentSpeedBar(float Percent);

private:
    UPROPERTY()
    ATG_HUD* TG_HUD;

private:
    void UpdateHUDVar();
};
