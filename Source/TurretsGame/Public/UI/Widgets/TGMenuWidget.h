// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurretGameTypes.h"
#include "TGMenuWidget.generated.h"

class UButton;
class USoundCue;

UCLASS()
class TURRETSGAME_API UTGMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UButton* PlayTankButton;

    UPROPERTY(meta = (BindWidget))
    UButton* PlayTurretButton;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    USoundCue* HoveredSound;

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnPlayTankButtonClicked();

    UFUNCTION()
    void OnPlayTurretButtonClicked();

    UFUNCTION()
    void OnPlayTankButtonHovered();

    UFUNCTION()
    void OnPlayTurretButtonHovered();

    void StartPlayByGameType(EGameType Type);
    void BindButtons();
    void PlayHoveredSound();
};
