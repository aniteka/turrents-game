// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "TGBasePawn.h"
#include "TurretGameTypes.h"
#include "TGTank.generated.h"

class UTGMovementComponent;
class UBoxComponent;

UCLASS()
class TURRETSGAME_API ATGTank : public ATGBasePawn
{
    GENERATED_BODY()

public:
    ATGTank();

    float GetSpeedPercent() const;
    
    bool IsPawnHidden() const { return PawnVisibility == EGameplayVisibility::EPGS_Hidden; };
    bool IsPawnVisible() const { return PawnVisibility == EGameplayVisibility::EPGS_Visible; };

    virtual void PostInitializeComponents() override;

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionInstance& Instance);
    virtual void ChangeTowerRotator() override;

    void SetPawnVisibility(AActor* OtherActor, EGameplayVisibility VisibilityState);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Foundation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UTGMovementComponent* MovementComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BushCollisionBox;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Move;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Gameplay")
    EGameplayVisibility PawnVisibility = EGameplayVisibility::EPGS_Visible;

private:
    UFUNCTION()
    void OnBushCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnBushCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};
