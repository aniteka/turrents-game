// TurretGame by Team #1. AlphaNova courses

#include "Player/TGBasePawn.h"
#include "Player/TGPlayerController.h"
#include "Components/TGShootComponent.h"
#include "Components/TGHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "GameMode/TGGameMode.h"
#include "AIController.h"

ATGBasePawn::ATGBasePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    ShootComp = CreateDefaultSubobject<UTGShootComponent>(TEXT("ShootComp"));
    HealthComp = CreateDefaultSubobject<UTGHealthComponent>(TEXT("HealthComp"));

    GroundBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("GroundBoxComp"));
    GroundBoxComp->SetupAttachment(GetRootComponent());

    Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
    Tower->SetCollisionObjectType(ECC_Pawn);
    Tower->SetupAttachment(GetRootComponent());

    Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
    Gun->SetupAttachment(Tower);
    Gun->SetCollisionObjectType(ECC_Pawn);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SetupAttachment(GetRootComponent());

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetRelativeLocation(FVector(0.f, -300.f, 0.f));
    CameraComp->SetupAttachment(SpringArmComp);

    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(GetRootComponent());

    SphereHit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereHit"));
    SphereHit->SetupAttachment(GetRootComponent());
    SphereHit->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATGBasePawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ChangeTowerRotator();
    ChangeGunRotator();
}

void ATGBasePawn::BeginPlay()
{
    Super::BeginPlay();

    Tags.Add(CustomTag);

    BindDelegates();
    UpdateHealthHUD();
}

void ATGBasePawn::BindDelegates()
{
    if (HealthComp)
    {
        HealthComp->OnDeathDelegate.AddDynamic(this, &ATGBasePawn::OnDeathCallback);
        HealthComp->OnHpChangeDelegate.AddDynamic(this, &ATGBasePawn::OnHpChangeCallback);
    }
    if (ShootComp)
    {
        ShootComp->OnShootDelegate.AddDynamic(this, &ATGBasePawn::OnShootCallback);
    }
}

void ATGBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    const APlayerController* PC = GetController<APlayerController>();
    if (!PC) return;

    const ULocalPlayer* LP = PC->GetLocalPlayer();
    if (!LP) return;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!Subsystem) return;

    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(DefaultInputMapping, 0);

    UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!InputComp) return;

    InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ATGBasePawn::PrimaryAttack);
    InputComp->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ATGBasePawn::Look);
    InputComp->BindAction(Input_Look, ETriggerEvent::Completed, this, &ATGBasePawn::StopLook);
    InputComp->BindAction(Input_Crosshair, ETriggerEvent::Triggered, this, &ATGBasePawn::CrosshairActivate);
    InputComp->BindAction(Input_Crosshair, ETriggerEvent::Completed, this, &ATGBasePawn::CrosshairDeactivate);
    InputComp->BindAction(Input_Pause, ETriggerEvent::Triggered, this, &ATGBasePawn::Pause);
    InputComp->BindAction(Input_ShootStrength, ETriggerEvent::Triggered, this, &ATGBasePawn::ShootStrength);
}

void ATGBasePawn::Look(const FInputActionValue& InputValue)
{
    if (IsDead()) return;

    const FVector2D Value = InputValue.Get<FVector2D>();

    AddControllerYawInput(Value.X);
    AddControllerPitchInput(-Value.Y);

    HandleLookSound(Value.X);
}

void ATGBasePawn::HandleLookSound(float ValueX)
{
    if (FMath::Abs(ValueX) > RotationSoundInputThreshold)
    {
        if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(RotationSoundTimerHandle)) return;

        GetWorld()->GetTimerManager().SetTimer(RotationSoundTimerHandle, RotationSoundCanPlayAgainTimer, false);
        StartRotateSound();
    }
    else
    {
        DeactivateRotationLoopComponent();
    }
}

void ATGBasePawn::StopLook(const FInputActionValue& InputValue)
{
    DeactivateRotationLoopComponent();
}

void ATGBasePawn::ChangeTowerRotator()
{
    if (!Tower || !SpringArmComp) return;

    FRotator TowerRot = Tower->GetRelativeRotation();
    TowerRot.Yaw = SpringArmComp->GetTargetRotation().Yaw;

    TowerRot = FMath::RInterpTo(Tower->GetRelativeRotation(), TowerRot, GetWorld()->DeltaTimeSeconds, TurnRate);

    Tower->SetRelativeRotation(TowerRot, true);
}

void ATGBasePawn::ChangeGunRotator()
{
    if (!Gun) return;

    FRotator GunRot = Gun->GetRelativeRotation();
    GunRot.Pitch = FMath::ClampAngle(SpringArmComp->GetTargetRotation().Pitch, -GunPitchThreshold, GunPitchThreshold);

    Gun->SetRelativeRotation(GunRot, true);
}

void ATGBasePawn::Pause()
{
    if (!UpdateTGPlayerControllerVar() || TGPlayerController->bGameOver || IsDead()) return;

    TGPlayerController->Pause();
}

void ATGBasePawn::OnDeathCallback(AActor* Actor)
{
    if (!Actor || Actor != this) return;

    PlayDeathVFX();
    NotifyGameModeAboutDeath();
}

void ATGBasePawn::OnHpChangeCallback(AActor* Actor, float NewHp, float Delta)
{
    if (!Actor || Actor != this) return;

    UpdateHealthHUD();
    TrySpawnFireBodyVFX();
}

void ATGBasePawn::UpdateHealthHUD()
{
    if (!UpdateTGPlayerControllerVar()) return;

    TGPlayerController->SetPercentHealthBar(GetHealthPercent());
}

void ATGBasePawn::NotifyGameModeAboutDeath()
{
    if (!GetWorld() || !UGameplayStatics::GetGameMode(GetWorld())) return;

    auto GameMode = Cast<ATGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (!GameMode) return;

    auto AIController = GetController<AAIController>();
    if (AIController)
    {
        GameMode->EnemyDestroyed(this);
        Destroy();
    }
    else
    {
        GameMode->GameOver(false);
    }
}

void ATGBasePawn::OnShootCallback(AActor* Actor)
{
    if (ShotSound && ShotSystem && Actor && Actor == this)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ShotSound, Gun->GetSocketLocation(GunShootSocketName));
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ShotSystem, Gun->GetSocketLocation(GunShootSocketName));
    }
}

void ATGBasePawn::TrySpawnFireBodyVFX()
{
    if (!BodyFireComponent && BodyFireSystem && GetHealthPercent() <= PercentToStartFire)
    {
        BodyFireComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(  //
            BodyFireSystem,                                                //
            GetRootComponent(),                                            //
            FName(),                                                       //
            GetActorLocation(),                                            //
            GetActorRotation(),                                            //
            EAttachLocation::KeepWorldPosition,                            //
            false                                                          //
        );
    }
}

void ATGBasePawn::PlayDeathVFX()
{
    if (!DestroySystem || !DestroyExplSystem) return;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestroySystem, GetActorLocation());
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestroyExplSystem, GetActorLocation());
}

void ATGBasePawn::PrimaryAttack()
{
    if (!ShootComp || !Gun || IsDead()) return;

    ShootComp->ShootFromComponent(Gun, GunShootSocketName);
}

void ATGBasePawn::StartRotateSound()
{
    if (RotationLoopSound && !RotationLoopComponent)
    {
        RotationLoopComponent = UGameplayStatics::SpawnSoundAttached(RotationLoopSound, GetRootComponent());
    }
}

void ATGBasePawn::DeactivateRotationLoopComponent()
{
    if (RotationLoopComponent)
    {
        RotationLoopComponent->Deactivate();
        RotationLoopComponent = nullptr;
    }
}

void ATGBasePawn::CrosshairActivate(const FInputActionValue& InputValue)
{
    if (IsDead()) return;

    ClearCrosshair();

    FPredictProjectilePathResult PredictResult;

    GetCrosshairPredictResult(PredictResult);
    DrawCrosshair(PredictResult);
    DrawEnemyHealthBar(PredictResult);
}

void ATGBasePawn::CrosshairDeactivate(const FInputActionValue& InputValue)
{
    if (!UpdateTGPlayerControllerVar() || IsDead()) return;

    TGPlayerController->EnableEnemyHealthBar(false);
    ClearCrosshair();
}

void ATGBasePawn::ShootStrength(const FInputActionInstance& Instance)
{
    if (!ShootComp || IsDead()) return;

    constexpr float ConvertPercentToValue = 100.f;
    const float AxisValue = Instance.GetValue().Get<float>();

    ShootSpeed = FMath::Clamp(
        ShootSpeed + AxisValue / ConvertPercentToValue * MouseInputSpeedMultiply, MinShootSpeedThreshold, MaxShootSpeedThreshold);

    static const float ProjectileSpeed = ShootComp->GetInitialProjectileSpeed();
    ShootComp->SetInitialProjectileSpeed(ProjectileSpeed * ShootSpeed);
}

void ATGBasePawn::GetCrosshairPredictResult(FPredictProjectilePathResult& PredictResult)
{
    if (!Gun || !ShootComp) return;

    const auto GunTransform = Gun->GetSocketTransform(GunShootSocketName);
    TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

    Objects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    Objects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
    Objects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

    FPredictProjectilePathParams PredictParams;
    PredictParams.ActorsToIgnore.Add(this);
    PredictParams.bTraceComplex = false;
    PredictParams.bTraceWithChannel = false;
    PredictParams.bTraceWithCollision = true;
    PredictParams.DrawDebugType = EDrawDebugTrace::None;
    PredictParams.LaunchVelocity = GunTransform.GetRotation().GetForwardVector() * ShootComp->GetInitialProjectileSpeed();
    PredictParams.MaxSimTime = 2.f;
    PredictParams.ObjectTypes = Objects;
    PredictParams.OverrideGravityZ = 0.f;
    PredictParams.ProjectileRadius = 20.f;
    PredictParams.SimFrequency = 15.f;
    PredictParams.StartLocation = GunTransform.GetLocation();

    UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);
}

void ATGBasePawn::DrawCrosshair(FPredictProjectilePathResult& PredictResult)
{
    if (!SplineComponent) return;

    for (int i = 0; i < PredictResult.PathData.Num(); ++i)
    {
        SplineComponent->AddSplinePointAtIndex(PredictResult.PathData[i].Location, i, ESplineCoordinateSpace::World);
    }

    if (SphereHit)
    {
        SphereHit->SetVisibility(true);
        SphereHit->SetWorldLocation(PredictResult.PathData.Last().Location);
    }

    SplineComponent->SetSplinePointType(PredictResult.PathData.Num() - 1, ESplinePointType::CurveClamped);

    for (int i = 0; i <= SplineComponent->GetNumberOfSplinePoints() - 2; ++i)
    {
        auto SplineMeshComponent =
            Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), true, FTransform(), false));
        if (!SplineMeshComponent) continue;

        SplineMeshComponent->SetMobility(EComponentMobility::Movable);

        if (SplineMesh && SplineMaterial)
        {
            SplineMeshComponent->SetStaticMesh(SplineMesh);
            SplineMeshComponent->SetMaterial(0, SplineMaterial);
        }

        SplineMeshComponent->SetStartScale(FVector2D(CrosshairDepth, CrosshairDepth));
        SplineMeshComponent->SetEndScale(FVector2D(CrosshairDepth, CrosshairDepth));

        PointsArray.Add(SplineMeshComponent);

        SplineMeshComponent->SetStartAndEnd(                                                //
            PredictResult.PathData[i].Location,                                             //
            SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World),     //
            PredictResult.PathData[i + 1].Location,                                         //
            SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World)  //
        );
    }
}

void ATGBasePawn::DrawEnemyHealthBar(FPredictProjectilePathResult& PredictResult)
{
    if (!UpdateTGPlayerControllerVar()) return;

    const auto CrosshairHitResult = PredictResult.HitResult;
    const bool bShowEnemyHealth = CrosshairHitResult.bBlockingHit && CrosshairHitResult.GetActor() &&
                                  !CrosshairHitResult.GetActor()->Tags.IsEmpty() && CrosshairHitResult.GetActor()->Tags[0] == CustomTag;
    if (bShowEnemyHealth)
    {
        const ATGBasePawn* TargetPawn = Cast<ATGBasePawn>(CrosshairHitResult.GetActor());
        if (!TargetPawn) return;

        TGPlayerController->SetPercentEnemyHealthBar(TargetPawn->GetHealthPercent());
        TGPlayerController->EnableEnemyHealthBar(true);
    }
    else
    {
        TGPlayerController->EnableEnemyHealthBar(false);
    }
}

void ATGBasePawn::ClearCrosshair()
{
    for (auto& ArrayItem : PointsArray)
    {
        if (!ArrayItem) continue;
        ArrayItem->DestroyComponent();
    }
    PointsArray.Empty();

    if (!SplineComponent) return;
    SplineComponent->ClearSplinePoints();

    if (!SphereHit) return;
    SphereHit->SetVisibility(false);
}

bool ATGBasePawn::UpdateTGPlayerControllerVar()
{
    TGPlayerController = (!TGPlayerController) ? GetController<ATGPlayerController>() : TGPlayerController;
    return (TGPlayerController) ? true : false;
}

float ATGBasePawn::GetHealthPercent() const
{
    if (!HealthComp) return 0.f;
    return HealthComp->GetHealthPercent();
}

float ATGBasePawn::GetShootDelayPercent() const
{
    if (!ShootComp) return 0.f;
    return ShootComp->GetShootDelayPercent();
}

bool ATGBasePawn::IsDead()
{
    if (!HealthComp) return false;
    return HealthComp->IsDead();
}
