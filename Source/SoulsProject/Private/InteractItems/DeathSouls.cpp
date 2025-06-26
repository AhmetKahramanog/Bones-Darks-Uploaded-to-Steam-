#include "InteractItems/DeathSouls.h"
#include <Kismet/GameplayStatics.h>
#include "MyCharacter.h"

ADeathSouls::ADeathSouls()
{
	PrimaryActorTick.bCanEverTick = true;

}



void ADeathSouls::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADeathSouls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeathSouls::Interact()
{
	AActor* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Character)
	{
		IIInteractable* ISoulTaker = Cast<IIInteractable>(Character);
		if (ISoulTaker)
		{
			ISoulTaker->GetSoulAmountFlex(DroppedSoulsAmount, 0);
			Destroy();
		}
	}
}

