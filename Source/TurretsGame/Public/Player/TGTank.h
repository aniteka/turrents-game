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
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PrimaryAttack() override;

    void Move(const FInputActionInstance& Instance);
    void Look(const FInputActionValue& InputValue);
    void ChangeTowerRotator();
    void ChangeGunRotator();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputMappingContext* DefaultInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Move;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Look;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TG|Components")
    UStaticMeshComponent* Foundation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TG|Components")
    UStaticMeshComponent* Tower;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TG|Components")
    UStaticMeshComponent* Gun;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TG|Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TG|Components")
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Distance")
    float TraceDistance;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Movement")
    float ForwardSpeed;
};
