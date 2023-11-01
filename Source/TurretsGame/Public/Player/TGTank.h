// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "TGBasePawn.h"
#include "TGTank.generated.h"

class UInputMappingContext;

UCLASS()
class TURRETSGAME_API ATGTank : public ATGBasePawn
{
    GENERATED_BODY()

public:
    ATGTank();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* Input_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* Foundation;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* Tower;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* Gun;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void PrimaryAttack() override;
    void Move(const FInputActionInstance& Instance);

};
