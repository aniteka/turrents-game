// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TGTank.generated.h"

UCLASS()
class TURRETSGAME_API ATGTank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATGTank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
