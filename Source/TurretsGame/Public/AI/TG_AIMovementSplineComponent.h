// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TG_AIMovementSplineComponent.generated.h"


class AAIController;
class AMovementSpline;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURRETSGAME_API UTG_AIMovementSplineComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTG_AIMovementSplineComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    AMovementSpline* GetMovementSpline() const;
    void SetMovementSpline(AMovementSpline* NewMovementSpline);

    FVector GetCurrentPointLocation() const;
    void NextPointLocation();

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG")
    TSoftObjectPtr<AMovementSpline> MovementSpline;

    UPROPERTY(EditAnywhere, Category = "TG")
    float CheckRadius = 100.f;

    UPROPERTY(EditAnywhere, Category = "TG")
    bool bCallMove = true;

private:
    int32 IndexMovementSpline = 0;

    AAIController* AIController = nullptr;

    void CheckCurrentPointLocation(float DeltaTime);
    void MoveToCurrentPoint(float DeltaTime);
};
