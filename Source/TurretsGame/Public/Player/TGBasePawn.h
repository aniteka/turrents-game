// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "TGBasePawn.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UTGShootComponent;
class UTGHealthComponent;
class UInputMappingContext;

UCLASS()
class TURRETSGAME_API ATGBasePawn : public APawn
{
    GENERATED_BODY()

public:
    ATGBasePawn();

    float GetHealthPercent() const;
    float GetShootDelayPercent() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UTGShootComponent* ShootComp;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UTGHealthComponent* HealthComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Tower;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Gun;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComp;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputMappingContext* DefaultInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Look;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_PrimaryAttack;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Threshold", meta = (Units = "Degrees"))
    float GunPitchThreshold = 20.f;

protected:
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;

    void Look(const FInputActionValue& InputValue);
    virtual void ChangeTowerRotator();
    virtual void ChangeGunRotator();

    virtual void PrimaryAttack();
};
