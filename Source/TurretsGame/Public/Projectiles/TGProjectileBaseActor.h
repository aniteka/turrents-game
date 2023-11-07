// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGProjectileBaseActor.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundCue;

UCLASS(Abstract, Blueprintable, BlueprintType)
class TURRETSGAME_API ATGProjectileBaseActor : public AActor
{
    GENERATED_BODY()

public:
    ATGProjectileBaseActor();

protected:
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;

public:
    FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
    FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
    
protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|Sounds")
    USoundCue* ExplosionSound;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|VFX")
    UNiagaraSystem* ExplosionSystem;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|VFX")
    UNiagaraSystem* SmokeExplSystem;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TG|VFX")
    float ExplosionScale = 1.f;

    UPROPERTY(EditAnywhere, Category = "TG")
    float HitDamage = 10.f;
    
private:
    UFUNCTION()
    void StaticMeshComponentEventHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
