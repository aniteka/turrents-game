// TurretGame by Team #1. AlphaNova courses


#include "AI/TG_AIController.h"

#include "Components/TGShootComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Player/TGBasePawn.h"

ATG_AIController::ATG_AIController()
{
    PrimaryActorTick.bCanEverTick = true;

    TGPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("TGPerceptionComponent");
}

void ATG_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    BasePawn = Cast<ATGBasePawn>(InPawn);
    GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ATG_AIController::PerceptionUpdatedCallback);
}

void ATG_AIController::OnUnPossess()
{
    Super::OnUnPossess();

    GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.Clear();
    BasePawn = nullptr;
}

void ATG_AIController::PawnStartShooting()
{
    if(!IsValid(BasePawn)) return;
    if(GetWorld()->GetTimerManager().TimerExists(ShootingTimerHandle)) return;

    GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &ATG_AIController::ShootingCallback,
        BasePawn->GetShootComponent()->GetShootDelayInSec(), true);
}

void ATG_AIController::PawnEndShooting()
{
    GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
}

void ATG_AIController::ShootingCallback()
{
    if(!IsValid(BasePawn)) return;
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
        PawnEndShooting();
        ClearFocus(EAIFocusPriority::LastFocusPriority);
        SetFocalPoint(Actor->GetActorLocation());
    }
}
