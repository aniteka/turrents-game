// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TGGameMode.generated.h"

class ATGPlayerController;

UENUM(BlueprintType)
enum class EGameType : uint8
{
    EGT_PlayTank UMETA(DisplayName = "Play Tank"),
    EGT_PlayTurret UMETA(DisplayName = "Play Turret")
};

UCLASS()
class TURRETSGAME_API ATGGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    ATGGameMode();

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    ATGPlayerController* TGPlayerController;

    UPROPERTY(EditDefaultsOnly, Category = "TG|GameMode properties")
    FTransform PlayerStartTransform;

    EGameType GameType = EGameType::EGT_PlayTank;

private:
    ATGPlayerController* GetTGPlayerController();
};
