// TurretGame by Team #1. AlphaNova courses


#include "Player/TGTank.h"

// Sets default values
ATGTank::ATGTank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATGTank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATGTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATGTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

