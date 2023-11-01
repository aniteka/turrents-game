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

    RestartTurretGame();
}

void ATGGameMode::EnemyDestroyed(AActor* EnemyToRemove)
{
    if (!EnemyToRemove) return;

    if (Enemies.Contains(EnemyToRemove))
    {
        Enemies.Remove(EnemyToRemove);
    }

    if (IsAllEnemiesDestroyed())
    {
        RestartGame();
    }
}

ATGPlayerController* ATGGameMode::GetTGPlayerController()
{
    return (!TGPlayerController) ? Cast<ATGPlayerController>(GetWorld()->GetFirstPlayerController()) : TGPlayerController;
}

bool ATGGameMode::IsAllEnemiesDestroyed()
{
    return Enemies.IsEmpty();
}

void ATGGameMode::SpawnPlayerByGameType()
{
    switch (GameType)
    {
        case EGameType::EGT_PlayTank: RestartPlayerAtTransform(GetTGPlayerController(), PlayerTankStartTransform); break;
        case EGameType::EGT_PlayTurret: RestartPlayerAtTransform(GetTGPlayerController(), PlayerTurretStartTransform); break;
    }
}

void ATGGameMode::SpawnEnemiesByGameType()
{
    switch (GameType)
    {
        case EGameType::EGT_PlayTank: SpawnActorsByTransforms(EnemyTurretClass, EnemyTurretsTransform); break;
        case EGameType::EGT_PlayTurret: SpawnActorsByTransforms(EnemyTankClass, EnemyTanksTransform); break;
    }
}

void ATGGameMode::SpawnActorsByTransforms(TSubclassOf<AActor>& InClass, const TArray<FTransform>& Transforms)
{
    auto World = GetWorld();
    if (!World) return;

    if (!World || !InClass || Transforms.IsEmpty()) return;

    for (const FTransform& TransformForSpawn : Transforms)
    {
        AActor* NewEnemy = World->SpawnActor<AActor>(InClass, TransformForSpawn);
        if (!NewEnemy) continue;

        Enemies.Add(NewEnemy);
    }
}

void ATGGameMode::RestartTurretGame()
{
    SpawnPlayerByGameType();
    SpawnEnemiesByGameType();
}
