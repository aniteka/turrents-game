// TurretGame by Team #1. AlphaNova courses

#include "GameMode/TGGameMode.h"
#include "Player/TGTank.h"
#include "Player/TGPlayerController.h"
#include "GameInstance/TGGameInstance.h"
#include "UI/HUD/TG_HUD.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

ATGGameMode::ATGGameMode()
{
    DefaultPawnClass = ATGTank::StaticClass();
    PlayerControllerClass = ATGPlayerController::StaticClass();
    HUDClass = ATG_HUD::StaticClass();
}

void ATGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    auto GameInstance = GetGameInstance<UTGGameInstance>();
    if (!GameInstance) return;

    GameType = GameInstance->ChoosedGameType;

    switch (GameType)
    {
        case EGameType::EGT_PlayTank: DefaultPawnClass = PlayerTankClass; break;
        case EGameType::EGT_PlayTurret: DefaultPawnClass = PlayerTurretClass; break;
    }
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
        GameOver(true);
    }
}

void ATGGameMode::GameOver(bool bWin)
{
    if (!GetTGPlayerController() || !GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(GameOverTimerHandle)) return;
    GetTGPlayerController()->GameOver(bWin);

    GetWorld()->GetTimerManager().SetTimer(GameOverTimerHandle, this, &ATGGameMode::OnGameOverTimerFinished, GameOverDelayInSec);
}

ATGPlayerController* ATGGameMode::GetTGPlayerController()
{
    return (!TGPlayerController) ? TGPlayerController = Cast<ATGPlayerController>(GetWorld()->GetFirstPlayerController())
                                 : TGPlayerController;
}

UTGGameInstance* ATGGameMode::GetTGGameInstance()
{
    return (!TGGameInstance) ? TGGameInstance = GetGameInstance<UTGGameInstance>() : TGGameInstance;
}

bool ATGGameMode::IsAllEnemiesDestroyed()
{
    return Enemies.IsEmpty();
}

void ATGGameMode::SpawnPlayerByGameType()
{
    if (!GetTGPlayerController()) return;

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
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        APawn* NewEnemy = World->SpawnActor<APawn>(InClass, TransformForSpawn, SpawnParams);
        if (!NewEnemy) continue;

        AAIController* EnemyController = NewEnemy->GetController<AAIController>();
        if (!EnemyController) continue;

        EnemyController->SetFocalPoint(NewEnemy->GetActorLocation() + NewEnemy->GetActorForwardVector());

        Enemies.Add(NewEnemy);
    }
}

void ATGGameMode::RestartTurretGame()
{
    SpawnPlayerByGameType();
    SpawnEnemiesByGameType();
}

void ATGGameMode::OnGameOverTimerFinished() 
{
    if (!GetTGGameInstance()) return;
    UGameplayStatics::OpenLevel(this, GetTGGameInstance()->MenuMapName);
}
