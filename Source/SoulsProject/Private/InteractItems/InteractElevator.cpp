#include "InteractItems/InteractElevator.h"

AInteractElevator::AInteractElevator()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AInteractElevator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractElevator::Interact()
{
	CanMovingPlatform = !CanMovingPlatform;
}

