// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementSpline.generated.h"

class USplineComponent;

UCLASS()
class TURRETSGAME_API AMovementSpline : public AActor
{
    GENERATED_BODY()

public:
    AMovementSpline();

    USplineComponent* GetSplineComponent() const { return SplineComponent; }

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
        USplineComponent* SplineComponent;
};
