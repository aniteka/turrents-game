// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "TGBasePawn.generated.h"

class UFloatingPawnMovement;
class UTGShootComponent;

UCLASS(Abstract)
class TURRETSGAME_API ATGBasePawn : public APawn
{
    GENERATED_BODY()

public:
    ATGBasePawn();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* Input_PrimaryAttack;

    // TODO: Future Updated
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UTGShootComponent* ShootComp;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UFloatingPawnMovement* MovementComp;

protected:
    virtual void PrimaryAttack() PURE_VIRTUAL(ATGBasePawn::PrimaryAttack,;);

};
