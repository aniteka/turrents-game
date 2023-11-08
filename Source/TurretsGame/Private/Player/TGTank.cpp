// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTank.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/TGMovementComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/TGShootComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ATGTank::ATGTank()
{
    PrimaryActorTick.bCanEverTick = true;

    Foundation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
    Foundation->SetSimulatePhysics(true);
    Foundation->SetCollisionObjectType(ECC_Pawn);
    Foundation->SetLinearDamping(1.f);
    SetRootComponent(Foundation);

    BushCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BushCollisionBox"));
    BushCollisionBox->SetupAttachment(GetRootComponent());

    GroundBoxComp->SetupAttachment(GetRootComponent());
    Tower->SetupAttachment(GetRootComponent());
    Gun->SetupAttachment(Tower);
    SpringArmComp->SetupAttachment(GetRootComponent());
    CameraComp->SetupAttachment(SpringArmComp);

    MovementComp = CreateDefaultSubobject<UTGMovementComponent>(TEXT("MovementComp"));
}

void ATGTank::BeginPlay()
{
    Super::BeginPlay();

    PlayIdleSound();
}

void ATGTank::Destroyed()
{
    if (IdleSoundComponent) IdleSoundComponent->Deactivate();
    if (StartMoveSoundComponent) StartMoveSoundComponent->Deactivate();
    if (MoveSoundComponent) MoveSoundComponent->Deactivate();
    if (RoadSmokeComponent) RoadSmokeComponent->Deactivate();
}

void ATGTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!InputComp) return;

    InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ATGTank::Move);
    InputComp->BindAction(Input_Move, ETriggerEvent::Completed, this, &ATGTank::StopMove);

}

void ATGTank::Move(const FInputActionInstance& Instance)
{
    if (!MovementComp) return;
    const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

    ActivateRoadSmokeSystem();
    PlayStartMoveSound();

    if (AxisValue.X != 0.f)
    {
        MovementComp->AddImpulseRotate(AxisValue.X);
    }

    if (AxisValue.Y != 0.f)
    {
        MovementComp->AddImpulse(AxisValue.Y);
    }
}

void ATGTank::StopMove(const FInputActionInstance& Instance)
{
    DeactivateRoadSmokeSystem();
    PlayIdleSound();
}

void ATGTank::ChangeTowerRotator()
{
    if (!Tower || !SpringArmComp || !Foundation) return;

    FRotator TowerRot = Tower->GetRelativeRotation();
    TowerRot.Yaw = SpringArmComp->GetTargetRotation().Yaw - Foundation->GetRelativeRotation().Yaw;

    TowerRot = FMath::RInterpTo(Tower->GetRelativeRotation(), TowerRot, GetWorld()->DeltaTimeSeconds, TurnRate);

    Tower->SetRelativeRotation(TowerRot, true);
}

void ATGTank::ActivateRoadSmokeSystem()
{
    if (!RoadSmokeSystem || RoadSmokeComponent || !MovementComp) return;

    RoadSmokeComponent = UNiagaraFunctionLibrary::SpawnSystemAttached( //
        RoadSmokeSystem,                                               //
        Foundation,                                                    //
        MovementComp->GetBackwardSocketName(),                         //
        GetActorLocation(),                                            //
        GetActorRotation(),                                            //
        EAttachLocation::KeepWorldPosition,                            //
        false                                                          //
        );
}

void ATGTank::DeactivateRoadSmokeSystem()
{
    if (!RoadSmokeComponent) return;

    RoadSmokeComponent->Deactivate();
    RoadSmokeComponent = nullptr;
}

void ATGTank::PlayIdleSound()
{
    if (IdleSound && !IdleSoundComponent)
    {
        IdleSoundComponent = UGameplayStatics::SpawnSoundAttached(IdleSound, GetRootComponent());
    }

    if (StartMoveSoundComponent && MoveSoundComponent)
    {
        StartMoveSoundComponent->Deactivate();
        StartMoveSoundComponent = nullptr;
        MoveSoundComponent->Deactivate();
        MoveSoundComponent = nullptr;
    }
}

void ATGTank::PlayStartMoveSound()
{
    if (StartMoveSound && MoveSound && !StartMoveSoundComponent && !MoveSoundComponent)
    {
        StartMoveSoundComponent = UGameplayStatics::SpawnSoundAttached(StartMoveSound, GetRootComponent());
        MoveSoundComponent = UGameplayStatics::SpawnSoundAttached(MoveSound, GetRootComponent());
    }

    if (IdleSoundComponent)
    {
        IdleSoundComponent->Deactivate();
        IdleSoundComponent = nullptr;
    }
}

float ATGTank::GetSpeedPercent() const
{
    if (!MovementComp) return 0.f;
    return MovementComp->GetPercentPower();
}

void ATGTank::ShootPayoff()
{
    // Invert Forward to Backward
    FVector BackwardVector = Gun->GetForwardVector() * -1;
    float ProjectileSpeed = ShootComp->GetInitialProjectileSpeed();
    Foundation->AddImpulse(BackwardVector * ProjectileSpeed * Foundation->GetMass() * ShootPayoffStrength);
}

void ATGTank::PrimaryAttack()
{
    if (!ShootComp) return;

    if (ShootComp->CanShootNow())
    {
        GetWorldTimerManager().SetTimer(TimerShootPayoff, this, &ATGTank::ShootPayoff, DelayShootPayoff);
    }

    Super::PrimaryAttack();
}
