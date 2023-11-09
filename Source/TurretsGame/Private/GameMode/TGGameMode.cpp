// TurretGame by Team #1. AlphaNova courses

#include "GameMode/TGGameMode.h"
#include "Player/TGTank.h"
#include "Player/TGPlayerController.h"
#include "GameInstance/TGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/TG_HUD.h"
#include "AI/TG_AIController.h"
#include "AI/TG_AIMovementSplineComponent.h"
#include "AI/MovementSpline.h"
#include "Components/SplineComponent.h"

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

void ATGGameMode::EnemyDestroyed(ATGBasePawn* EnemyToRemove)
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
        case EGameType::EGT_PlayTank:  //
            SpawnActorsByTransforms(EnemyTurretClass, EnemyTurretsTransform);
            break;
        case EGameType::EGT_PlayTurret:  //
            SpawnActorsByTransforms(EnemyTankClass, EnemyTanksTransform);
            SetPathsForEnemyTanks();
            break;
    }
}

void ATGGameMode::SpawnActorsByTransforms(TSubclassOf<ATGBasePawn>& InClass, const TArray<FTransform>& Transforms)
{
    auto World = GetWorld();
    if (!World) return;

    if (!World || !InClass || Transforms.IsEmpty()) return;

    for (const FTransform& TransformForSpawn : Transforms)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ATGBasePawn* NewEnemy = World->SpawnActor<ATGBasePawn>(InClass, TransformForSpawn, SpawnParams);
        if (!NewEnemy) continue;

        AAIController* EnemyController = NewEnemy->GetController<AAIController>();
        if (!EnemyController) continue;

        EnemyController->SetFocalPoint(NewEnemy->GetActorLocation() + NewEnemy->GetActorForwardVector());

        Enemies.Add(NewEnemy);
    }
}

void ATGGameMode::SetPathsForEnemyTanks()
{
    if (Enemies.Num() != EnemyTanksPoints.Num()) return;

    int i = 0;
    for (auto& EnemyTank : Enemies)
    {
        ATG_AIController* EnemyTankController = EnemyTank->GetController<ATG_AIController>();
        if (!EnemyTankController || !EnemyTankController->GetAIMovementSplineComponent()) return;

        AMovementSpline* MovementSpline = NewObject<AMovementSpline>();
        if (!MovementSpline || !MovementSpline->GetSplineComponent()) return;

        MovementSpline->GetSplineComponent()->SetSplinePoints(EnemyTanksPoints[i].Path, ESplineCoordinateSpace::World);
        EnemyTankController->GetAIMovementSplineComponent()->SetMovementSpline(MovementSpline);

        ++i;
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
