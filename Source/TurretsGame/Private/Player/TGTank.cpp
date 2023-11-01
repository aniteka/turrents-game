// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTank.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/TGShootComponent.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), false, TEXT("Enable Debug Lines for Change Rotation of Gun Component."), ECVF_Cheat);

ATGTank::ATGTank()
{
    PrimaryActorTick.bCanEverTick = true;

    Foundation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
    Foundation->SetSimulatePhysics(true);
    RootComponent = Foundation;

    Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
    Tower->SetAbsolute(false, true);
    Tower->SetupAttachment(Foundation);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
    Gun->SetupAttachment(Tower);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SetupAttachment(Foundation);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp);

    TraceDistance = 1'000.0f;
    ForwardSpeed = 500.0f;
}

void ATGTank::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ChangeTowerRotator();
    ChangeGunRotator();
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
    ShootComp->ShootFromComponent(Gun);
    UE_LOG(LogTemp, Log, TEXT("[%s] Shooting!!!"), *GetNameSafe(this));
}

void ATGTank::Move(const FInputActionInstance& Instance)
{
    const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

    if (AxisValue.X != 0.0f)
    {
        FRotator CurrentRotation = Foundation->GetComponentRotation();
        FRotator NewRotation = CurrentRotation + FRotator(0.0f, AxisValue.X, 0.0f);
        
        Foundation->SetRelativeRotation(NewRotation);
    }

    if (AxisValue.Y != 0.0f)
    {
        FVector ForwardVector = Foundation->GetForwardVector();
        FVector MovementDirection = ForwardVector * AxisValue.Y;

        Foundation->AddImpulse(MovementDirection * ForwardSpeed * GetWorld()->DeltaTimeSeconds, NAME_None, true);
    }
}

void ATGTank::Look(const FInputActionValue& InputValue)
{
    const FVector2D Value = InputValue.Get<FVector2D>();

    AddControllerYawInput(Value.X);
    AddControllerPitchInput(Value.Y);
}

void ATGTank::ChangeTowerRotator()
{
    FRotator TowerRot = Tower->GetRelativeRotation();
    TowerRot.Roll = 0.0f;
    TowerRot.Pitch = 0.0f;
    
    TowerRot.Yaw = SpringArmComp->GetTargetRotation().Yaw;
    
    Tower->SetRelativeRotation(TowerRot, true);
}

void ATGTank::ChangeGunRotator()
{
    FRotator GunRot = Tower->GetRelativeRotation();
    GunRot.Pitch = SpringArmComp->GetTargetRotation().Pitch;

    Tower->SetRelativeRotation(GunRot, true);
}

bool ATGTank::GetResultFromLineTrace(const FVector& StartPoint, const FVector& EndPoint, FHitResult& HitResult)
{
    FCollisionObjectQueryParams CollisionParams;
    CollisionParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    CollisionParams.AddObjectTypesToQuery(ECC_WorldStatic);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    return GetWorld()->LineTraceSingleByObjectType(HitResult, StartPoint, EndPoint, CollisionParams, Params);
}
