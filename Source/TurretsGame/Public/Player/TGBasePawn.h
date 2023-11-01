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
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UTGShootComponent* ShootComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Tower;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Gun;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_PrimaryAttack;

protected:
    virtual void PrimaryAttack() PURE_VIRTUAL(ATGBasePawn::PrimaryAttack, ;);
};
