// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TGOverlayWidget.generated.h"

class UProgressBar;

UCLASS()
class TURRETSGAME_API UTGOverlayWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    void NativeConstruct() override;

public:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ShootDelayBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* SpeedBar;
};
