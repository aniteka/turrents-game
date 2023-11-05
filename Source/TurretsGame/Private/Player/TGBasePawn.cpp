// TurretGame by Team #1. AlphaNova courses

#include "Player/TGBasePawn.h"
#include "Player/TGPlayerController.h"
#include "Components/TGShootComponent.h"
#include "Components/TGHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

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

void ATGBasePawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ChangeTowerRotator();
    ChangeGunRotator();
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
    InputComp->BindAction(Input_Crosshair, ETriggerEvent::Triggered, this, &ATGBasePawn::CrosshairActivate);
    InputComp->BindAction(Input_Crosshair, ETriggerEvent::Completed, this, &ATGBasePawn::CrosshairDeactivate);
}

void ATGBasePawn::BeginPlay()
{
    Super::BeginPlay();
}

void ATGBasePawn::Look(const FInputActionValue& InputValue)
{
    const FVector2D Value = InputValue.Get<FVector2D>();

    AddControllerYawInput(Value.X);
    AddControllerPitchInput(Value.Y);
}

void ATGBasePawn::ChangeTowerRotator()
{
    if (!Tower || !SpringArmComp) return;

    FRotator TowerRot = Tower->GetRelativeRotation();
    TowerRot.Yaw = SpringArmComp->GetTargetRotation().Yaw;

    Tower->SetRelativeRotation(TowerRot, true);
}

void ATGBasePawn::ChangeGunRotator()
{
    if (!Gun) return;

    FRotator GunRot = Gun->GetRelativeRotation();
    GunRot.Pitch = FMath::ClampAngle(SpringArmComp->GetTargetRotation().Pitch, -GunPitchThreshold, GunPitchThreshold);

    Gun->SetRelativeRotation(GunRot, true);
}

void ATGBasePawn::PrimaryAttack()
{
    if (!ShootComp || !Gun) return;
    ShootComp->ShootFromComponent(Gun, FName(TEXT("ShootSocket")));
    ShootComp->DrawCrosshair(Gun);
}

void ATGBasePawn::CrosshairActivate(const FInputActionValue& InputValue)
{
    ClearCrosshair();

    if (!SplineComponent) return;

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
    PredictParams.LaunchVelocity = GunTransform.GetRotation().GetForwardVector() * 5000.f;
    PredictParams.MaxSimTime = 2.f;
    PredictParams.ObjectTypes = Objects;
    PredictParams.OverrideGravityZ = 0.f;
    PredictParams.ProjectileRadius = 20.f;
    PredictParams.SimFrequency = 15.f;
    PredictParams.StartLocation = GunTransform.GetLocation();

    FPredictProjectilePathResult PredictResult;
    UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

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

void ATGBasePawn::CrosshairDeactivate(const FInputActionValue& InputValue) 
{
    ClearCrosshair();
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
