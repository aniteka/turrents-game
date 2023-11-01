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
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputMappingContext* DefaultInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Move;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Look;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Components")
    UStaticMeshComponent* Foundation;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Components")
    UStaticMeshComponent* Tower;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Components")
    UStaticMeshComponent* Gun;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Components")
    UCameraComponent* CameraComp;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void PrimaryAttack() override;
    void Move(const FInputActionInstance& Instance);
    void Look(const FInputActionValue& InputValue);

    void ChangeTowerRotator();
    void ChangeGunRotator();

};
