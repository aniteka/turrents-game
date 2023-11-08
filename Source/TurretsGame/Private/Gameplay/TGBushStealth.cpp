// TurretGame by Team #1. AlphaNova courses

#include "Gameplay/TGBushStealth.h"
#include "Components/SphereComponent.h"

ATGBushStealth::ATGBushStealth()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(MeshComponent);

    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SphereCollision->SetupAttachment(GetRootComponent());
}
