// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TGShootComponent.generated.h"

class ATGProjectileBaseActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TURRETSGAME_API UTGShootComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTGShootComponent();

public:
    UFUNCTION(BlueprintCallable, Category = "TG|Shoot")
    ATGProjectileBaseActor* ShootFromLocation(FVector Location, FVector ShootDirection);
    UFUNCTION(BlueprintCallable, Category = "TG|Shoot", meta = (AdvancedDisplay = "ShootDirection, Socket"))
    ATGProjectileBaseActor* ShootFromActor(AActor* Actor, FName Socket = NAME_None, FVector ShootDirection = FVector::ZeroVector);
    UFUNCTION(BlueprintCallable, Category = "TG|Shoot", meta = (AdvancedDisplay = "ShootDirection, Socket"))
    ATGProjectileBaseActor* ShootFromComponent(USceneComponent* Component, FName Socket = NAME_None,
        FVector ShootDirection = FVector::ZeroVector);

protected:
    UPROPERTY(EditAnywhere, Category = "TG")
    TSubclassOf<ATGProjectileBaseActor> ProjectileClass;
    UPROPERTY(EditAnywhere, Category = "TG", DisplayName = "Impulse Multiplier")
    float ProjectileImpulseMultiplier = 1000.f;
    UPROPERTY(EditAnywhere, Category = "TG", DisplayName = "Socket To Apply Impulse")
    FName ProjectileSocketToApplyImpulse = NAME_None;
    UPROPERTY(EditAnywhere, Category = "TG", DisplayName = "Set Life Span After Spawn?")
    bool bProjectileSetLifeSpanAfterSpawn = false;
    UPROPERTY(EditAnywhere, Category = "TG", DisplayName = "Life Span In Sec",
        meta = (EditCondition = "bProjectileSetLifeSpanAfterSpawn", EditConditionHides))
    float ProjectileLifeSpanInSec = 10.f;

};
