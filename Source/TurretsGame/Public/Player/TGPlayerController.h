// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "TGPlayerController.generated.h"

class ATG_HUD;

UCLASS()
class TURRETSGAME_API ATGPlayerController
    : public APlayerController
    , public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    void SetPercentHealthBar(float Percent);

protected:
    virtual void OnPossess(APawn* InPawn) override;
    
private:
    UPROPERTY()
    ATG_HUD* TG_HUD;

private:
    void UpdateHUDVar();
};
