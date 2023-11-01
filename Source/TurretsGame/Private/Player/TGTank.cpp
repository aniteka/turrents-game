// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTank.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATGTank::ATGTank()
{
    PrimaryActorTick.bCanEverTick = true;

    Foundation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
    Foundation->SetSimulatePhysics(true);
    RootComponent = Foundation;

    Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
    Tower->SetupAttachment(Foundation);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
    Gun->SetupAttachment(Tower);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SetupAttachment(Tower);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp);
}

void ATGTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    const APlayerController* PC = GetController<APlayerController>();
    const ULocalPlayer* LP = PC->GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(Subsystem);

    Subsystem->ClearAllMappings();

    Subsystem->AddMappingContext(DefaultInputMapping, 0);

    UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    InputComp->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ATGTank::PrimaryAttack);
    InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ATGTank::Move);
    InputComp->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ATGTank::Look);
}

void ATGTank::PrimaryAttack()
{
    // TODO: Future Tank Attack
    // ShootComp->
    UE_LOG(LogTemp, Log, TEXT("[%s] Shooting!!!"), *GetNameSafe(this));
}

void ATGTank::Move(const FInputActionInstance& Instance)
{
    const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();
    const FVector Direct{AxisValue.X, AxisValue.Y, 0.0f};

    FVector ForwardVector = Foundation->GetForwardVector();

    FRotator Rot = CameraComp->GetComponentRotation();
    Rot.Pitch = 0.0f;
    Rot.Roll = 0.0f;
}

void ATGTank::Look(const FInputActionValue& InputValue)
{
    const FVector2D Value = InputValue.Get<FVector2D>();

    AddControllerYawInput(Value.X);
    AddControllerPitchInput(Value.Y);
}
