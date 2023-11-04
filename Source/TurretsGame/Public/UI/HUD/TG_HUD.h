// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TG_HUD.generated.h"

class UTGMenuWidget;

UCLASS()
class TURRETSGAME_API ATG_HUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Widgets")
    TSubclassOf<UUserWidget> MenuWidgetClass;

public:
    void AddMenuWidget();

private:
    UPROPERTY()
    UTGMenuWidget* MenuWidget;
};
