// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TGHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHpChangeDelegate, AActor*, Actor, float, NewHp, float, Delta);

class ATGPlayerController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURRETSGAME_API UTGHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTGHealthComponent();

    float GetHealthPercent() const;

protected:
    virtual void BeginPlay() override;

public:
    FORCEINLINE float GetMaxHp() const { return MaxHp; }
    FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

    UFUNCTION(BlueprintGetter)
    FORCEINLINE float GetHp() const { return Hp; }

    UFUNCTION(BlueprintSetter)
    void SetHp(float NewHp);

    UFUNCTION(BlueprintCallable, Category = "TG")
    void AddDeltaToHp(float Delta);

    FORCEINLINE bool IsDead() const { return bIsDead; }

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG", meta = (ClampMin = 0, UIMax = 100))
    float MaxHp = 100.f;

public:
    UPROPERTY(BlueprintAssignable, Category = "Delegates")
    FOnDeathDelegate OnDeathDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Delegates")
    FOnHpChangeDelegate OnHpChangeDelegate;

private:
    UPROPERTY(EditAnywhere, BlueprintSetter = SetHp, BlueprintGetter = GetHp, Category = "TG", meta = (AllowPrivateAccess = 1))
    float Hp = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "TG", meta = (AllowPrivateAccess = 1))
    bool bIsDead = false;

    UPROPERTY()
    ATGPlayerController* TGPlayerController;

private:
    void DeathCheck(float InHp);

    UFUNCTION()
    void OnTakeAnyDamageCallback(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
