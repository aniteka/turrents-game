// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Player/TGBasePawn.h"
#include "TG_AIController.generated.h"

struct FActorPerceptionUpdateInfo;

UCLASS()
class TURRETSGAME_API ATG_AIController : public AAIController
{
    GENERATED_BODY()

public:
    ATG_AIController();

    virtual UAIPerceptionComponent* GetPerceptionComponent() override { return TGPerceptionComponent; };
    
protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    UAIPerceptionComponent* TGPerceptionComponent;
    
private:
    UFUNCTION()
    void PerceptionUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo);

    void PawnStartShooting();
    void PawnEndShooting();
    void ShootingCallback();
    
    ATGBasePawn* BasePawn = nullptr;
    FTimerHandle ShootingTimerHandle;
};
