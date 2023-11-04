// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TG_HUD.generated.h"

class UTGMenuWidget;
class UTGOverlayWidget;
class ATGBasePawn;

UCLASS()
class TURRETSGAME_API ATG_HUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Widgets")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Widgets")
    TSubclassOf<UUserWidget> OverlayWidgetClass;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    void AddMenuWidget();
    void AddOverlayWidget();

    void EnableHealthBar();
    void EnableShootDelayBar();
    void EnableSpeedBar();

    void SetPercentHealthBar(float Percent);
    void SetPercentShootDelayBar(float Percent);
    void SetPercentSpeedBar(float Percent);

private:
    UPROPERTY()
    UTGMenuWidget* MenuWidget;

    UPROPERTY()
    UTGOverlayWidget* OverlayWidget;

    UPROPERTY()
    ATGBasePawn* OwnerPawn;

private:
    void OverlayWidgetCreateHandle();
    void UpdateShootDelayBar();
};
