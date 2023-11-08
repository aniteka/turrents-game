// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "TGPlayerController.generated.h"

class ATG_HUD;
class UImage;
class USoundCue;

UCLASS()
class TURRETSGAME_API ATGPlayerController
    : public APlayerController
    , public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    void Pause();
    void GoToMenu();
    void SetInputModeGameOnly();
    void SetInputModeUIOnly();

    void EnableEnemyHealthBar(bool bEnable);
    void SetPercentHealthBar(float Percent);
    void SetPercentEnemyHealthBar(float Percent);

protected:
    virtual void OnPossess(APawn* InPawn) override;
    
private:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    USoundCue* BattleTheme;

    UPROPERTY()
    ATG_HUD* TG_HUD;

private:
    void SetPercentBar(float Percent, UImage* BarToChange);
    void UpdateHUDVar();
    void PlayMusic();
};
