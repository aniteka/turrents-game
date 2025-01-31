// TurretGame by Team #1. AlphaNova courses

#include "Components/TGMovementComponent.h"
#include "Components/BoxComponent.h"

void UTGMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    if (ensureMsgf(GetPawnOwner(), TEXT("TGMovementComponent didn't have Pawn!")) &&
        ensureMsgf(GetWorld(), TEXT("TGMovementComponent didn't have World!")))
    {
        return;
    }
}

void UTGMovementComponent::AddImpulseMovement(float DeltaTime)
{
    if (FMath::IsNearlyZero(GetLastPowerInput()))
    {
        if (!FMath::IsNearlyZero(Power))
        {
            Power = FMath::FInterpTo(Power, 0.f, DeltaTime, InterpSpeed);
        }
        return;
    }

    if (!HasGroundContact() || !UpdatedComponent) return;

    if (GetPawnOwner()->GetVelocity().Size() >= VelocityThreshold) return;

    Power = FMath::Clamp(Power + GetLastPowerInput(), -PowerThreshold, PowerThreshold);

    UStaticMeshComponent* MyMeshComp = Cast<UStaticMeshComponent>(UpdatedComponent);
    if (!MyMeshComp) return;

    const FVector ForwardVector = MyMeshComp->GetForwardVector();
    const FVector Impulse = ForwardVector * ImpulseStrength * DeltaTime * Power;

    MyMeshComp->AddImpulse(Impulse, NAME_None, true);
}

void UTGMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AddImpulseMovement(DeltaTime);
}

FTimerManager& UTGMovementComponent::GetWorldTimerManager() const
{
    return GetWorld()->GetTimerManager();
}

void UTGMovementComponent::ResetLastPowerInput_Elapsed()
{
    GetWorldTimerManager().ClearTimer(ResetLastPowerInput);
    LastPowerInput = 0.f;
}

void UTGMovementComponent::AddImpulse(float PowerInput)
{
    constexpr float ConvertPercentToValue = 100.f;
    const float PowerEngineInput = PowerInput / ConvertPercentToValue;

    LastPowerInput = PowerEngineInput;

    if (GetWorldTimerManager().IsTimerActive(ResetLastPowerInput)) GetWorldTimerManager().ClearTimer(ResetLastPowerInput);

    GetWorldTimerManager().SetTimer(
        ResetLastPowerInput, this, &UTGMovementComponent::ResetLastPowerInput_Elapsed, DelayResetLastPowerInput);
}

void UTGMovementComponent::AddImpulseRotate(float PowerInput)
{
    if (!HasGroundContact() || FMath::IsNearlyZero(GetPowerEngine()) || !UpdatedComponent) return;

    UStaticMeshComponent* MyMeshComp = Cast<UStaticMeshComponent>(UpdatedComponent);
    if (!MyMeshComp) return;

    // Invert value to correct working with socket settings, when object moving forward or backward
    PowerInput *= Power > 0 ? -1 : 1;

    const FVector ForwardSocketDirection = MyMeshComp->GetSocketRotation(ForwardSocketName).Vector() * PowerInput;
    const FVector BackwardSocketDirection = MyMeshComp->GetSocketRotation(BackwardSocketName).Vector() * PowerInput;

    MyMeshComp->AddImpulseAtLocation(ForwardSocketDirection * SidewaysSpeed, MyMeshComp->GetSocketLocation(ForwardSocketName));
    MyMeshComp->AddImpulseAtLocation(BackwardSocketDirection * SidewaysSpeed, MyMeshComp->GetSocketLocation(BackwardSocketName));
}

void UTGMovementComponent::RequestMoveTo(FVector Location)
{
    UStaticMeshComponent* FoundationComponent = Cast<UStaticMeshComponent>(GetPawnOwner()->GetRootComponent());
    if (!FoundationComponent) return;

    auto CurrentFoundationFwd = FoundationComponent->GetForwardVector();
    CurrentFoundationFwd.Z = 0.f;
    CurrentFoundationFwd.Normalize();

    auto TargetDirection = Location - FoundationComponent->GetComponentLocation();
    TargetDirection.Z = 0.f;
    TargetDirection.Normalize();

    AddImpulse(RequestMovementImpulseForward);

    const double Angle = FMath::Acos(FVector::DotProduct(TargetDirection, CurrentFoundationFwd));
    float ImpulseRotate =
        FVector::CrossProduct(TargetDirection, CurrentFoundationFwd).Z <= 0.0 ? RequestMovementImpulseRotate : -RequestMovementImpulseRotate;

    if (FMath::RadiansToDegrees(Angle) > ConcentrateAngle)
    {
        AddImpulseRotate(ImpulseRotate);
    }
}

bool UTGMovementComponent::HasGroundContact() const
{
    const APawn* ControlledPawn = GetPawnOwner();
    if (!ControlledPawn) return false;

    const UBoxComponent* BoxComp = ControlledPawn->FindComponentByClass<UBoxComponent>();
    if (!BoxComp) return false;

    const FVector Start = BoxComp->GetComponentLocation();

    // Move down to the height of the box plus a small additional space.
    const FVector End = Start - FVector(0, 0, BoxComp->GetScaledBoxExtent().Z + 1.0f);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(ControlledPawn);

    FHitResult Hit;
    const bool bHit = GetWorld()->SweepSingleByChannel(
        Hit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeBox(BoxComp->GetScaledBoxExtent()), QueryParams);

    return bHit;
}

float UTGMovementComponent::GetPercentPower() const
{
    return FMath::Clamp(GetPawnOwner()->GetVelocity().Size(), 0.f, VelocityThreshold) / VelocityThreshold;
}
