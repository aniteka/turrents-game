// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Player/TGBasePawn.h"
#include "TGTurret.generated.h"

UCLASS()
class TURRETSGAME_API ATGTurret : public ATGBasePawn
{
    GENERATED_BODY()

public:
    ATGTurret();

    virtual void PostInitializeComponents() override;

private:
    UFUNCTION()
    void OnTakePointDamageCallback(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
        class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
        AActor* DamageCauser);
};
