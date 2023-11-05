// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Components/TGShootComponent.h"
#include "TG_AIShootComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories = "TG|Delay|DeferredShot")
class TURRETSGAME_API UTG_AIShootComponent : public UTGShootComponent
{
    GENERATED_BODY()

public:
    UTG_AIShootComponent();

    
protected:
    virtual ATGProjectileBaseActor* ShootImplementation(const FInfoForShoot& Info) override;    
};
