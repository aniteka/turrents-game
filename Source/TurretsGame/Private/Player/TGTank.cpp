// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTank.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/TGMovementComponent.h"
#include "Engine/StaticMeshSocket.h"

ATGTank::ATGTank()
{
    PrimaryActorTick.bCanEverTick = true;

    Foundation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
    Foundation->SetSimulatePhysics(true);
    Foundation->SetCollisionObjectType(ECC_Pawn);
    SetRootComponent(Foundation);

    BoxComp->SetupAttachment(GetRootComponent());
    Tower->SetupAttachment(GetRootComponent());
    Gun->SetupAttachment(Tower);
    SpringArmComp->SetupAttachment(GetRootComponent());
    CameraComp->SetupAttachment(SpringArmComp);

    MovementComp = CreateDefaultSubobject<UTGMovementComponent>(TEXT("MovementComp"));
}

void ATGTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!InputComp) return;

    InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ATGTank::Move);
}

void ATGTank::Move(const FInputActionInstance& Instance)
{
    if (!Foundation) return;
    const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

    if (AxisValue.X != 0.f && MovementComp->HasGroundContact())
    {
        FRotator CurrentRotation = Foundation->GetComponentRotation();
        FRotator NewRotation = CurrentRotation + FRotator(0.f, AxisValue.X, 0.f);
        Foundation->SetRelativeRotation(NewRotation);
    }

    if (AxisValue.Y != 0.f)
    {
        MovementComp->AddImpulse(AxisValue.Y);
    }
}

void ATGTank::ChangeTowerRotator()
{
    if (!Tower || !SpringArmComp || !Foundation) return;

    FRotator TowerRot = Tower->GetRelativeRotation();
    TowerRot.Yaw = SpringArmComp->GetTargetRotation().Yaw - Foundation->GetRelativeRotation().Yaw;

    Tower->SetRelativeRotation(TowerRot, true);
}
