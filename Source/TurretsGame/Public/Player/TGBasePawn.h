// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "TGBasePawn.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UTGShootComponent;
class UTGHealthComponent;
class UInputMappingContext;
class ATGPlayerController;
class USplineComponent;
class USplineMeshComponent;
class USoundCue;
class UAudioComponent;
class UNiagaraSystem;
class UNiagaraComponent;

struct FPredictProjectilePathResult;

UCLASS()
class TURRETSGAME_API ATGBasePawn : public APawn
{
    GENERATED_BODY()

public:
    ATGBasePawn();

    FORCEINLINE UTGShootComponent* GetShootComponent() const { return ShootComp; }
    FORCEINLINE UTGHealthComponent* GetHealthComponent() const { return HealthComp; }
    FORCEINLINE UStaticMeshComponent* GetGunStaticMeshComponent() const { return Gun; }

    virtual void PrimaryAttack();
    virtual float GetHealthPercent() const;
    virtual float GetShootDelayPercent() const;
    virtual float GetSpeedPercent() const { return 0.f; };

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UTGShootComponent* ShootComp;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UTGHealthComponent* HealthComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Tower;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Gun;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UBoxComponent* GroundBoxComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USplineComponent* SplineComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SphereHit;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputMappingContext* DefaultInputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Look;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_PrimaryAttack;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Crosshair;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_Pause;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Input")
    UInputAction* Input_ShootStrength;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Threshold", meta = (Units = "Degrees"))
    float GunPitchThreshold = 20.f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sockets")
    FName GunShootSocketName = FName(TEXT("ShootSocket"));

    UPROPERTY(EditDefaultsOnly, Category = "TG|Crosshair")
    float CrosshairDepth = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Crosshair")
    UStaticMesh* SplineMesh;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Crosshair")
    UMaterial* SplineMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Crosshair")
    FName CustomTag = FName(TEXT("TGBasePawn"));

    UPROPERTY(EditDefaultsOnly, Category = "TG|Tower")
    float TurnRate = 1.f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Strength")
    float ShootSpeed = 1.f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Strength")
    float MouseInputSpeedMultiply = 3.f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Strength")
    float MinShootSpeedThreshold = 0.2f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Strength")
    float MaxShootSpeedThreshold = 1.f;

protected:
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;

    void Look(const FInputActionValue& InputValue);
    void StopLook(const FInputActionValue& InputValue);
    void CrosshairActivate(const FInputActionValue& InputValue);
    void CrosshairDeactivate(const FInputActionValue& InputValue);
    void ShootStrength(const FInputActionInstance& Instance);
    virtual void ChangeTowerRotator();
    virtual void ChangeGunRotator();
    void Pause();

private:
    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    USoundCue* ShotSound;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    USoundCue* RotationLoopSound;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    float RotationSoundInputThreshold = 0.03f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    float RotationSoundCanPlayAgainTimer = 0.5f;

    UPROPERTY(EditDefaultsOnly, Category = "TG|VFX")
    float PercentToStartFire = 0.4;

    UPROPERTY(EditDefaultsOnly, Category = "TG|VFX")
    UNiagaraSystem* DestroySystem;

    UPROPERTY(EditDefaultsOnly, Category = "TG|VFX")
    UNiagaraSystem* DestroyExplSystem;

    UPROPERTY(EditDefaultsOnly, Category = "TG|VFX")
    UNiagaraSystem* ShotSystem;

    UPROPERTY(EditDefaultsOnly, Category = "TG|VFX")
    UNiagaraSystem* BodyFireSystem;

    UPROPERTY()
    UNiagaraComponent* BodyFireComponent;

    UPROPERTY()
    UAudioComponent* RotationLoopComponent;

    UPROPERTY()
    ATGPlayerController* TGPlayerController;

    TArray<USplineMeshComponent*> PointsArray;
    FTimerHandle RotationSoundTimerHandle;

private:
    UFUNCTION()
    void OnDeathCallback(AActor* Actor);

    UFUNCTION()
    void OnShootCallback(AActor* Actor);

    UFUNCTION()
    void OnHpChangeCallback(AActor* Actor, float NewHp, float Delta);

    void NotifyGameModeAboutDeath();
    void UpdateHealthHUD();
    void TrySpawnFireBodyVFX();
    void PlayDeathVFX();
    void GetCrosshairPredictResult(FPredictProjectilePathResult& PredictResult);
    void DrawCrosshair(FPredictProjectilePathResult& PredictResult);
    void DrawEnemyHealthBar(FPredictProjectilePathResult& PredictResult);
    void ClearCrosshair();
    void StartRotateSound();
    void DeactivateRotationLoopComponent();
    void BindDelegates();
    void HandleLookSound(float ValueX);
    bool UpdateTGPlayerControllerVar();
};
