// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TG_HUD.generated.h"

class UTGMenuWidget;
class UTGOverlayWidget;
class ATGBasePawn;
class UImage;
class ATGPlayerController;

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

    UPROPERTY()
    UTGMenuWidget* MenuWidget;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    void Pause(bool bPaused);
    void GameOver(bool bWin);

    void EnableBar(UImage* BarToEnable, bool bEnable);
    void SetPercentBar(float Percent, UImage* BarToChange);

    void AddMenuWidget();
    void AddOverlayWidget();

private:
    UPROPERTY(EditDefaultsOnly, Category = "TG|HUD")
    FText WinMessage = FText::FromString(TEXT("YOU WIN!"));

    UPROPERTY(EditDefaultsOnly, Category = "TG|HUD")
    FText LoseMessage = FText::FromString(TEXT("YOU LOSE!"));

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
    void ShowPauseComponents(const ESlateVisibility& Visibility);
    ATGPlayerController* GetTGPlayerController();
};
