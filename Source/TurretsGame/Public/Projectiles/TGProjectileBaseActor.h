// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGProjectileBaseActor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;

UCLASS(Abstract, Blueprintable, BlueprintType)
class TURRETSGAME_API ATGProjectileBaseActor : public AActor
{
    GENERATED_BODY()

public:
    ATGProjectileBaseActor();

    virtual void BeginPlay() override;

protected:
    virtual void PostInitializeComponents() override;

public:
    FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
    FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    USphereComponent* SphereCollision;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|Sounds")
    USoundCue* ExplosionSound;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|VFX")
    UNiagaraSystem* ExplosionSystem;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|VFX")
    UNiagaraSystem* SmokeExplSystem;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|VFX")
    UNiagaraSystem* TrailSystem;

    UPROPERTY()
    UNiagaraComponent* TrailComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|VFX")
    float ExplosionScale = 1.f;

    UPROPERTY(EditAnywhere, Category = "TG")
    float HitDamage = 10.f;

    UPROPERTY(EditAnywhere, Category = "TG")
    float LifeSpanAfterHit = 5.f;

private:
    UFUNCTION()
    void StaticMeshComponentEventHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        FVector NormalImpulse, const FHitResult& Hit);

    void SpawnDestroyCosmetics();
    void SpawnTrailSystem();
    void DeactivateAndDestroy();
};
