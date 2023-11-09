// TurretGame by Team #1. AlphaNova courses

#include "AI/TG_AIMovementSplineComponent.h"
#include "AITypes.h"
#include "AI/MovementSpline.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "Components/TGMovementComponent.h"

UTG_AIMovementSplineComponent::UTG_AIMovementSplineComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTG_AIMovementSplineComponent::BeginPlay()
{
    Super::BeginPlay();

    AIController = GetOwner<AAIController>();
    check(AIController);
}

void UTG_AIMovementSplineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    CheckCurrentPointLocation(DeltaTime);
    MoveToCurrentPoint(DeltaTime);
}

AMovementSpline* UTG_AIMovementSplineComponent::GetMovementSpline() const
{
    return MovementSpline;
}

void UTG_AIMovementSplineComponent::SetMovementSpline(AMovementSpline* NewMovementSpline)
{
    if (MovementSpline == NewMovementSpline || !NewMovementSpline) return;

    IndexMovementSpline = 0;
    MovementSpline = NewMovementSpline;
}

FVector UTG_AIMovementSplineComponent::GetCurrentPointLocation() const
{
    if (!MovementSpline || !MovementSpline->GetSplineComponent()) return FAISystem::InvalidLocation;
    return MovementSpline->GetSplineComponent()->GetLocationAtSplinePoint(IndexMovementSpline, ESplineCoordinateSpace::World);
}

void UTG_AIMovementSplineComponent::NextPointLocation()
{
    if (!MovementSpline || !MovementSpline->GetSplineComponent()) return;

    if (++IndexMovementSpline >= MovementSpline->GetSplineComponent()->GetNumberOfSplinePoints())
    {
        IndexMovementSpline = 0;
    }
}

void UTG_AIMovementSplineComponent::CheckCurrentPointLocation(float DeltaTime)
{
    if (!AIController || !AIController->GetPawn()) return;

    if ((GetCurrentPointLocation() - AIController->GetPawn()->GetActorLocation()).Length() <= CheckRadius)
    {
        NextPointLocation();
    }
}

void UTG_AIMovementSplineComponent::MoveToCurrentPoint(float DeltaTime)
{
    if (!bCallMove || !AIController || !AIController->GetPawn() || !AIController->GetPawn()->GetMovementComponent()) return;

    const auto TGMovementComponent = Cast<UTGMovementComponent>(AIController->GetPawn()->GetMovementComponent());
    if (!TGMovementComponent) return;

    if (const FVector CurrentPointLocation = GetCurrentPointLocation(); CurrentPointLocation != FAISystem::InvalidLocation)
    {
        TGMovementComponent->RequestMoveTo(CurrentPointLocation);
    }
}
