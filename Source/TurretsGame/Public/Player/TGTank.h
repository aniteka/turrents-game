// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "TGBasePawn.h"
#include "TurretGameTypes.h"
#include "TGTank.generated.h"

class UTGMovementComponent;
class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;
class UAudioComponent;

UCLASS()
class TURRETSGAME_API ATGTank : public ATGBasePawn
{
    GENERATED_BODY()

public:
    ATGTank();

    virtual void BeginPlay() override;
    virtual void Destroyed() override;

    float GetSpeedPercent() const;

    virtual bool IsPawnHidden() const override { return PawnVisibility == EGameplayVisibility::EPGS_Hidden; };
    virtual bool IsPawnVisible() const override { return PawnVisibility == EGameplayVisibility::EPGS_Visible; };

    virtual void PostInitializeComponents() override;

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionInstance& Instance);
    void StopMove(const FInputActionInstance& Instance);
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
    void OnBushCollisionEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sound")
    USoundCue* IdleSound;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sound")
    USoundCue* StartMoveSound;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sound")
    USoundCue* MoveSound;

    UPROPERTY()
    UAudioComponent* IdleSoundComponent;

    UPROPERTY()
    UAudioComponent* StartMoveSoundComponent;

    UPROPERTY()
    UAudioComponent* MoveSoundComponent;

    UPROPERTY(EditDefaultsOnly, Category = "TG|VFX")
    UNiagaraSystem* RoadSmokeSystem;

    UPROPERTY()
    UNiagaraComponent* RoadSmokeComponent;

private:
    void ActivateRoadSmokeSystem();
    void DeactivateRoadSmokeSystem();

    void PlayIdleSound();
    void PlayStartMoveSound();
};
