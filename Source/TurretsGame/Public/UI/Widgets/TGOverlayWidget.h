// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TGOverlayWidget.generated.h"

class UProgressBar;
class UImage;

UCLASS()
class TURRETSGAME_API UTGOverlayWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    void NativeConstruct() override;

public:
    UPROPERTY(meta = (BindWidget))
    UImage* HealthBarImage;

    UPROPERTY(meta = (BindWidget))
    UImage* ShootDelayBarImage;

    UPROPERTY(meta = (BindWidget))
    UImage* SpeedBarImage;

    UPROPERTY(meta = (BindWidget))
    UImage* EnemyHealthBarImage;
};
