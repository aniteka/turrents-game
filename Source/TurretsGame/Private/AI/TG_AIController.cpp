// TurretGame by Team #1. AlphaNova courses

#include "AI/TG_AIController.h"

#include "AI/TG_AIMovementSplineComponent.h"
#include "AI/TG_AITeams.h"
#include "Components/TGShootComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Player/TGBasePawn.h"

ATG_AIController::ATG_AIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    TGPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("TGPerceptionComponent");
    GetRootComponent()->SetUsingAbsoluteRotation(false);
    SetPerceptionComponent(*TGPerceptionComponent);

    AIMovementSplineComponent = CreateDefaultSubobject<UTG_AIMovementSplineComponent>("AIMovementSplineComponent");
}

ETeamAttitude::Type ATG_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (const APawn* OtherPawn = Cast<APawn>(&Other))
    {
        if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
        {
            return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
        }
    }
    return Super::GetTeamAttitudeTowards(Other);
}

void ATG_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    SetGenericTeamId(TGTeamId::EnemyId);
    BasePawn = Cast<ATGBasePawn>(InPawn);
    GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ATG_AIController::PerceptionUpdatedCallback);
}

void ATG_AIController::OnUnPossess()
{
    Super::OnUnPossess();

    SetGenericTeamId(FGenericTeamId::NoTeam);
    GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.Clear();
    BasePawn = nullptr;
}

void ATG_AIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
    if (!BasePawn || !BasePawn->GetShootComponent()) return;

    FVector Dir;
    UGameplayStatics::SuggestProjectileVelocity(GetWorld(), Dir, BasePawn->GetActorLocation(), GetFocalPoint(),
        BasePawn->GetShootComponent()->GetInitialProjectileSpeed(), false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);

    const auto OldControlRotation = GetControlRotation();
    const auto TargetControlRotation = Dir.GetSafeNormal().Rotation();
    const auto NewControlRotation =
        UKismetMathLibrary::RInterpTo_Constant(OldControlRotation, TargetControlRotation, DeltaTime, GunRotationInterpSpeed);

    SetControlRotation(NewControlRotation);
}

void ATG_AIController::PawnStartShooting()
{
    if (!BasePawn || !bCanShoot) return;
    if (GetWorld()->GetTimerManager().TimerExists(ShootingTimerHandle)) return;

    GetWorld()->GetTimerManager().SetTimer(
        ShootingTimerHandle, this, &ATG_AIController::ShootingCallback, BasePawn->GetShootComponent()->GetShootDelayInSec(), true);
}

void ATG_AIController::PawnEndShooting()
{
    if(!bCanShoot) return;
    GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
}

void ATG_AIController::EndShooting(AActor* Actor)
{
    if (!Actor) return;

    PawnEndShooting();
    ClearFocus(EAIFocusPriority::LastFocusPriority);
    SetFocalPoint(Actor->GetActorLocation());
}

void ATG_AIController::ShootingCallback()
{
    if (!BasePawn) return;
    BasePawn->PrimaryAttack();
}

void ATG_AIController::PerceptionUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    if (!UpdateInfo.Target.IsValid()) return;

    const auto Actor = UpdateInfo.Target.Get();

    if (UpdateInfo.Stimulus.WasSuccessfullySensed())
    {
        SetFocus(Actor);
        PawnStartShooting();
    }
    else
    {
        EndShooting(Actor);
    }
}
