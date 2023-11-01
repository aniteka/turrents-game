// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTank.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"

ATGTank::ATGTank()
{
    PrimaryActorTick.bCanEverTick = true;

    Foundation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
    RootComponent = Foundation;

    Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
    Tower->SetupAttachment(Foundation);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
    Gun->SetupAttachment(Tower);

    MovementComp->SetUpdatedComponent(Foundation);
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
    InputComp->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ATGTank::PrimaryAttack);
}

void ATGTank::PrimaryAttack()
{
    Super::PrimaryAttack();

    // TODO: Future Tank Attack
    // ShootComp->
    UE_LOG(LogTemp, Log, TEXT("[%s] Shooting!!!"), *GetNameSafe(this));
}

void ATGTank::Move(const FInputActionInstance& Instance)
{
}
