// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TurretGameTypes.h"
#include "TGMenuPlayerController.generated.h"

class USoundCue;

UCLASS()
class TURRETSGAME_API ATGMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    void StartPlayByGameType(EGameType Type);

private:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    USoundCue* MenuTheme;

private:
    void SetInputModeUIOnly();
    void PlayMusic();
};
