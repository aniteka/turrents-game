// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGProjectileBaseActor.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class TURRETSGAME_API ATGProjectileBaseActor : public AActor
{
    GENERATED_BODY()

public:
    ATGProjectileBaseActor();

protected:
    virtual void BeginPlay() override;

public:
    UStaticMeshComponent* GetStaticMeshComponent() const { return GetComponentByClass<UStaticMeshComponent>(); }

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;
};
