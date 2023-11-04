// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TGMovementComponent.generated.h"

UCLASS()
class TURRETSGAME_API UTGMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * Increase the power of the engine to move
     * @param PowerInput Expected Value from Key Input [-1.0, 1.0]
     */
    void AddImpulse(float PowerInput);

    virtual bool HasGroundContact() const;

    /** Returned percent, 0 -> 1 moving forward, 0 -> -1 moving back */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    FORCEINLINE float GetPowerEngine() const { return Power; };

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Movement")
    float PowerThreshold = 1.f;

    /** Displays the maximum engine power from -1 to 1 */
    UPROPERTY(VisibleAnywhere, Category = "TG|Movement", meta = (Units = "Percent"))
    float Power = 0.f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Movement")
    float ImpulseStrength = 1200.f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Movement|Engine Braking")
    float InterpSpeed = 0.25f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Movement|Engine Braking")
    float VelocityThreshold = 700.f;

    FTimerHandle ResetLastPowerInput;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Movement|Engine Braking")
    float DelayResetLastPowerInput = 0.05f;

protected:
    FORCEINLINE float GetLastPowerInput() const { return LastPowerInput; };

private:
    float LastPowerInput = 0.f;

private:
    FTimerManager& GetWorldTimerManager() const;

    void ResetLastPowerInput_Elapsed();

    void AddImpulseMovement(float DeltaTime);
};
