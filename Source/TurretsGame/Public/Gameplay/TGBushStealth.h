// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGBushStealth.generated.h"

class USphereComponent;

UCLASS()
class TURRETSGAME_API ATGBushStealth : public AActor
{
    GENERATED_BODY()

public:
    ATGBushStealth();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    USphereComponent* SphereCollision;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;
};
