// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "TGBasePawn.h"
#include "TGTank.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;

UCLASS()
class TURRETSGAME_API ATGTank : public ATGBasePawn
{
    GENERATED_BODY()

public:
    ATGTank();

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionInstance& Instance);
    virtual void ChangeTowerRotator() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Foundation;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Move;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Distance")
    float TraceDistance = 1000.f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Movement")
    float ForwardSpeed = 500.f;
};
