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

    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
    virtual void StartPlay() override;

private:
    UPROPERTY()
    ATGPlayerController* TGPlayerController;

    UPROPERTY(EditDefaultsOnly, Category = "TG|GameMode properties")
    FTransform PlayerTankStartTransform;

    UPROPERTY(EditDefaultsOnly, Category = "TG|GameMode properties")
    FTransform PlayerTurretStartTransform;

    UPROPERTY(EditDefaultsOnly, Category = "TG|GameMode properties")
    TArray<FTransform> EnemyTurretsTransform;

    UPROPERTY(EditDefaultsOnly, Category = "TG|GameMode properties")
    TArray<FTransform> EnemyTanksTransform;

    UPROPERTY(EditDefaultsOnly, Category = "TG|GameMode properties")
    TSubclassOf<AActor> EnemyTankClass;

    UPROPERTY(EditDefaultsOnly, Category = "TG|GameMode properties")
    TSubclassOf<AActor> EnemyTurretClass;

    EGameType GameType = EGameType::EGT_PlayTank;

private:
    ATGPlayerController* GetTGPlayerController();

    void SpawnActorsByTransforms(TSubclassOf<AActor>& InClass, const TArray<FTransform>& Transforms);
};
