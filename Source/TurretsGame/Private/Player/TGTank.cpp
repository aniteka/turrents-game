// TurretGame by Team #1. AlphaNova courses

#include "Player/TGTank.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), false, TEXT("Enable Debug Lines for Change Rotation of Gun Component."), ECVF_Cheat);

ATGTank::ATGTank()
{
    PrimaryActorTick.bCanEverTick = true;

    Foundation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
    Foundation->SetSimulatePhysics(true);
    RootComponent = Foundation;

    Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
    Tower->SetupAttachment(Foundation);

    Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
    Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
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

    if (AxisValue.X != 0.0f)
    {
        FRotator CurrentRotation = Foundation->GetComponentRotation();
        FRotator NewRotation = CurrentRotation + FRotator(0.0f, AxisValue.X, 0.0f);
        Foundation->SetWorldRotation(NewRotation);
    }

    if (AxisValue.Y != 0.0f)
    {
        float Speed = 500.0f;
        
        FVector ForwardVector = Foundation->GetForwardVector();
        FVector MovementDirection = ForwardVector * AxisValue.Y;
        SetActorLocation(GetActorLocation() + MovementDirection * Speed * GetWorld()->DeltaTimeSeconds);
    }
}

void ATGTank::Look(const FInputActionValue& InputValue)
{
    const FVector2D Value = InputValue.Get<FVector2D>();

    AddControllerYawInput(Value.X);
    AddControllerPitchInput(Value.Y);

    ChangeTowerRotator();
    ChangeGunRotator();
}

void ATGTank::ChangeTowerRotator()
{
    FRotator TowerRot = Tower->GetComponentRotation();
    TowerRot.Yaw = CameraComp->GetComponentRotation().Yaw;

    Tower->SetWorldRotation(TowerRot, true);
}

void ATGTank::ChangeGunRotator()
{
    float DistanceTrace = 1'000.0f;
    FVector StartPoint = CameraComp->GetComponentLocation();
    FVector EndPoint = StartPoint + CameraComp->GetComponentRotation().Vector() * DistanceTrace;

    FCollisionObjectQueryParams CollisionParams;
    CollisionParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    CollisionParams.AddObjectTypesToQuery(ECC_WorldStatic);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    FHitResult HitResult;

    GetWorld()->LineTraceSingleByObjectType(HitResult, StartPoint, EndPoint, CollisionParams, Params);

    FVector Direction = (HitResult.GetActor() ? HitResult.ImpactPoint : EndPoint) - Gun->GetComponentLocation();
    FRotator TargetRotation = FRotationMatrix::MakeFromYZ(Direction, Direction).Rotator();

    Gun->SetWorldRotation(TargetRotation);

    if (CVarDebugDrawInteraction.GetValueOnGameThread())
    {
        DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 2.0f);
        DrawDebugCircle(GetWorld(), HitResult.GetActor() ? HitResult.ImpactPoint : EndPoint, 32.0f, 16, FColor::Blue, false, 0.1f);
    }
}
