// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "TGBasePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTGShootComponent;

UCLASS(Abstract)
class TURRETSGAME_API ATGBasePawn : public APawn
{
    GENERATED_BODY()

public:
    ATGBasePawn();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UTGShootComponent* ShootComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Tower;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Gun;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComp;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_PrimaryAttack;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Threshold", meta = (Units = "Degrees"))
    float GunPitchThreshold = 20.f;
    
protected:
    virtual void PrimaryAttack() PURE_VIRTUAL(ATGBasePawn::PrimaryAttack, ;);
    virtual void ChangeTowerRotator();
    virtual void ChangeGunRotator();
};
