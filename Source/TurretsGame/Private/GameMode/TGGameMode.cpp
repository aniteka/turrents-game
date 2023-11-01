// TurretGame by Team #1. AlphaNova courses

#include "GameMode/TGGameMode.h"
#include "Player/TGTank.h"
#include "Player/TGPlayerController.h"

ATGGameMode::ATGGameMode()
{
    DefaultPawnClass = ATGTank::StaticClass();
    PlayerControllerClass = ATGPlayerController::StaticClass();
}

void ATGGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    if (!NewPlayer) return;
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);

    switch (GameType)
    {
        case EGameType::EGT_PlayTank: RestartPlayerAtTransform(GetTGPlayerController(), PlayerTankStartTransform); break;
        case EGameType::EGT_PlayTurret: RestartPlayerAtTransform(GetTGPlayerController(), PlayerTurretStartTransform); break;
    }
}

void ATGGameMode::StartPlay()
{
    Super::StartPlay();

    auto World = GetWorld();
    if (!World) return;

    switch (GameType)
    {
        case EGameType::EGT_PlayTank: SpawnActorsByTransforms(EnemyTurretClass, EnemyTurretsTransform); break;
        case EGameType::EGT_PlayTurret: SpawnActorsByTransforms(EnemyTankClass, EnemyTanksTransform); break;
    }
}

ATGPlayerController* ATGGameMode::GetTGPlayerController()
{
    return (!TGPlayerController) ? Cast<ATGPlayerController>(GetWorld()->GetFirstPlayerController()) : TGPlayerController;
}

void ATGGameMode::SpawnActorsByTransforms(TSubclassOf<AActor>& InClass, const TArray<FTransform>& Transforms)
{
    auto World = GetWorld();
    if (!World) return;

    if (!World || !InClass || Transforms.IsEmpty()) return;

    for (const FTransform& TransformForSpawn : Transforms)
    {
        World->SpawnActor<AActor>(InClass, TransformForSpawn);
    }
}
