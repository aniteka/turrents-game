// TurretGame by Team #1. AlphaNova courses

#include "AI/MovementSpline.h"
#include "Components/SplineComponent.h"

AMovementSpline::AMovementSpline()
{
    PrimaryActorTick.bCanEverTick = false;

    SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}
