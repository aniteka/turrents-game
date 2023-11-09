// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Player/TGBasePawn.h"
#include "TG_AIController.generated.h"

class UTG_AIMovementSplineComponent;
struct FActorPerceptionUpdateInfo;

UCLASS()
class TURRETSGAME_API ATG_AIController : public AAIController
{
    GENERATED_BODY()

public:
    ATG_AIController(const FObjectInitializer& ObjectInitializer);

    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
    
protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

    virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    UAIPerceptionComponent* TGPerceptionComponent;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    UTG_AIMovementSplineComponent* AIMovementSplineComponent;
    
protected:
    UPROPERTY(EditAnywhere, Category = "TG")
    float GunRotationInterpSpeed = 30.f;
    UPROPERTY(EditAnywhere, Category = "TG")
    bool bCanShoot = true;
    
private:
    UFUNCTION()
    void PerceptionUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo);

    void PawnStartShooting();
    void PawnEndShooting();
    void EndShooting(AActor* Actor);
    void ShootingCallback();

    ATGBasePawn* BasePawn = nullptr;
    FTimerHandle ShootingTimerHandle;
};
