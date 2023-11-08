// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TG_HUD.generated.h"

class UTGMenuWidget;
class UTGOverlayWidget;
class ATGBasePawn;
class UImage;

UCLASS()
class TURRETSGAME_API ATG_HUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Widgets")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Widgets")
    TSubclassOf<UUserWidget> OverlayWidgetClass;

    UPROPERTY()
    UTGOverlayWidget* OverlayWidget;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    void Pause(bool bPaused);

    void EnableBar(UImage* BarToEnable, bool bEnable);
    void SetPercentBar(float Percent, UImage* BarToChange);

    void AddMenuWidget();
    void AddOverlayWidget();

private:
    UPROPERTY()
    UTGMenuWidget* MenuWidget;

    UPROPERTY()
    ATGBasePawn* OwnerPawn;

private:
    UFUNCTION()
    void OnResumeButtonClicked();

    UFUNCTION()
    void OnMenuButtonClicked();

    void SelectWidgetByGameMode();
    void OverlayWidgetCreateHandle();
    bool UpdateOwnerPawnVar();
};
