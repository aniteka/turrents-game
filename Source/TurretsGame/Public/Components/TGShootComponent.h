// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TGShootComponent.generated.h"

class ATGProjectileBaseActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShoot, AActor*, Actor);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURRETSGAME_API UTGShootComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTGShootComponent();

    UPROPERTY(BlueprintAssignable, Category = "Delegates")
    FOnShoot OnShootDelegate;

protected:
    struct FInfoForShoot
    {
        FVector Location;
        FVector Direction;
    };

public:
    UFUNCTION(BlueprintCallable, Category = "TG|Shoot")
    ATGProjectileBaseActor* ShootFromLocation(FVector Location, FVector ShootDirection);

    UFUNCTION(BlueprintCallable, Category = "TG|Shoot", meta = (AdvancedDisplay = "ShootDirection, Socket"))
    ATGProjectileBaseActor* ShootFromActor(AActor* Actor, FName Socket = NAME_None, FVector ShootDirection = FVector::ZeroVector);

    UFUNCTION(BlueprintCallable, Category = "TG|Shoot", meta = (AdvancedDisplay = "ShootDirection, Socket"))
    ATGProjectileBaseActor* ShootFromComponent(
        USceneComponent* Component, FName Socket = NAME_None, FVector ShootDirection = FVector::ZeroVector);

    UFUNCTION(BlueprintPure, Category = "TG|Shoot")
    bool CanShootNow() const;

    UFUNCTION(BlueprintPure, Category = "TG|Shoot|Delay")
    bool IsShootDelay() const;

    UFUNCTION(BlueprintPure, Category = "TG|Shoot|Delay")
    float GetRemainsOfShootDelay() const;

    FORCEINLINE TSubclassOf<ATGProjectileBaseActor> GetProjectileClass() const { return ProjectileClass; }
    FORCEINLINE void SetProjectileClass(TSubclassOf<ATGProjectileBaseActor> NewProjectileClass) { ProjectileClass = NewProjectileClass; }

    FORCEINLINE float GetInitialProjectileSpeed() const { return InitialProjectileSpeed; }
    FORCEINLINE void SetInitialProjectileSpeed(float NewInitialProjectileSpeed) { InitialProjectileSpeed = NewInitialProjectileSpeed; }

    FORCEINLINE float GetShootDelayInSec() const { return ShootDelayInSec; }
    FORCEINLINE void SetShootDelayInSec(float NewShootDelayInSec) { ShootDelayInSec = NewShootDelayInSec; }

    float GetShootDelayPercent() const;

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG")
    TSubclassOf<ATGProjectileBaseActor> ProjectileClass;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG")
    float InitialProjectileSpeed = 5000.f;

    UPROPERTY(EditAnywhere, Category = "TG", DisplayName = "Socket To Apply Impulse")
    FName ProjectileSocketToApplyImpulse = NAME_None;

    UPROPERTY(EditAnywhere, Category = "TG", DisplayName = "Set Life Span After Spawn?")
    bool bProjectileSetLifeSpanAfterSpawn = true;

    UPROPERTY(EditAnywhere, Category = "TG", DisplayName = "Life Span In Sec",
        meta = (EditCondition = "bProjectileSetLifeSpanAfterSpawn", EditConditionHides))
    float ProjectileLifeSpanInSec = 10.f;

    UPROPERTY(EditAnywhere, Category = "TG|Delay", DisplayName = "Use Shoot Delay?")
    bool bUseShootDelay = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|Delay", meta = (EditCondition = "bUseShootDelay", EditConditionHides))
    float ShootDelayInSec = 1.5f;

    // TURN OFF FOR AI
    UPROPERTY(EditAnywhere, Category = "TG|Delay|DeferredShot", DisplayName = "Use Deferred Shot?",
        meta = (EditCondition = "bUseShootDelay", EditConditionHides))
    bool bUseDeferredShot = false;

    // -1 means use time from ShootDelayInSec
    UPROPERTY(EditAnywhere, Category = "TG|Delay|DeferredShot", meta = (EditCondition = "bUseDeferredShot", EditConditionHides))
    float RemainingTimeForDeferredShot = -1.f;

protected:
    virtual ATGProjectileBaseActor* ShootImplementation(const FInfoForShoot& Info);

private:
    FTimerHandle ShootDelayTimerHandle;
    bool bShootImmediatelyAfterDelay = false;
    FInfoForShoot AfterDelayInfo;

    void ShootDelayCallback();

    /**
     * @return true - can shoot | false - cant
     */
    bool PreShootCheck(const FInfoForShoot& Info);
};
