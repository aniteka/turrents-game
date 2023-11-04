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

    if (!HasGroundContact()) return;

    Power = FMath::Clamp(Power + GetLastPowerInput(), -PowerThreshold, PowerThreshold);

    UStaticMeshComponent* MyMeshComp = Cast<UStaticMeshComponent>(GetPawnOwner()->GetRootComponent());
    if (!MyMeshComp) return;

    const FVector ForwardVector = GetPawnOwner()->GetActorForwardVector();
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
    constexpr float ConvertToPercent = 100.f;
    const float PowerEngineInput = PowerInput / ConvertToPercent;

    LastPowerInput = PowerEngineInput;

    if (GetWorldTimerManager().IsTimerActive(ResetLastPowerInput))
        GetWorldTimerManager().ClearTimer(ResetLastPowerInput);

    GetWorldTimerManager().SetTimer(ResetLastPowerInput, this, &UTGMovementComponent::ResetLastPowerInput_Elapsed,
        DelayResetLastPowerInput);
}

bool UTGMovementComponent::HasGroundContact() const
{
    APawn* ControlledPawn = GetPawnOwner();
    if (!ControlledPawn) return false;

    UBoxComponent* BoxComp = ControlledPawn->FindComponentByClass<UBoxComponent>();
    if (!BoxComp) return false;

    FVector Start = BoxComp->GetComponentLocation();
    // Move down to the height of the box plus a small additional space.
    FVector End = Start - FVector(0, 0, BoxComp->GetScaledBoxExtent().Z + 1.0f);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(ControlledPawn);

    FHitResult Hit;
    bool bHit = GetWorld()->SweepSingleByChannel(
        Hit,
        Start,
        End,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeBox(BoxComp->GetScaledBoxExtent()),
        QueryParams
        );

    return bHit;
}
