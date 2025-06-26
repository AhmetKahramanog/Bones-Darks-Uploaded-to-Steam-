#include "InteractItems/LockedDoor.h"
#include "InteractableKey.h"
#include <Menu/MySaveGame.h>
#include <Kismet/GameplayStatics.h>

ALockedDoor::ALockedDoor()
{
	PrimaryActorTick.bCanEverTick = true;

}



void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();

	if (AssignedKey)
	{
		AInteractableKey* Key = Cast<AInteractableKey>(AssignedKey);
		if (Key)
		{
			Key->OnKeyPickedUp.AddDynamic(this, &ALockedDoor::OpenDoor);
			UMySaveGame* LoadGameObject = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("KeySaveSlot"), 0));
			if (LoadGameObject && LoadGameObject->SavedKeys.Contains(Key->GetFName()))
			{
				CanOpen = true;
			}
		}
	}

	CurrentRotation = GetActorRotation();
	TargetRotation = CurrentRotation + FRotator(95.f,0.f,0.f);
	
}

void ALockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpenDoor)
	{
		FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 2.f);
		SetActorRotation(SmoothRotation);
	}

}

void ALockedDoor::OpenDoor(AInteractableKey* Key)
{
	CanOpen = true;
}

void ALockedDoor::Interact()
{
	if (CanOpen)
	{
		IsOpenDoor = true;
		GetWorld()->GetTimerManager().SetTimer(DestroyDoorHandle, FTimerDelegate::CreateLambda([this]
			{
				Destroy();
			}), 6.f, false);
	}
}

