// TurretGame by Team #1. AlphaNova courses

#include "Player/TGBasePawn.h"
#include "Components/TGShootComponent.h"
#include "Components/TGHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATGBasePawn::ATGBasePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    ShootComp = CreateDefaultSubobject<UTGShootComponent>(TEXT("ShootComp"));
    HealthComp = CreateDefaultSubobject<UTGHealthComponent>(TEXT("HealthComp"));

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
    CameraComp->SetupAttachment(SpringArmComp);
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
    ShootComp->ShootFromComponent(Gun);
    ShootComp->DrawCrosshair(Gun);
}
