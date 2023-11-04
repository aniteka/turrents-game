// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurretGameTypes.h"
#include "TGMenuWidget.generated.h"

class UButton;

UCLASS()
class TURRETSGAME_API UTGMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UButton* PlayTankButton;

    UPROPERTY(meta = (BindWidget))
    UButton* PlayTurretButton;

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnPlayTankButtonClicked();

    UFUNCTION()
    void OnPlayTurretButtonClicked();

    void StartPlayByGameType(EGameType Type);
};
