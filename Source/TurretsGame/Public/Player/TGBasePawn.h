// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "TGBasePawn.generated.h"

class UTGShootComponent;

UCLASS(Abstract)
class TURRETSGAME_API ATGBasePawn : public APawn
{
    GENERATED_BODY()

public:
    ATGBasePawn();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_PrimaryAttack;

    // TODO: Future Updated
    UPROPERTY(EditDefaultsOnly, Category = "TG|Components")
    UTGShootComponent* ShootComp;

protected:
    virtual void PrimaryAttack() PURE_VIRTUAL(ATGBasePawn::PrimaryAttack, ;);

};
