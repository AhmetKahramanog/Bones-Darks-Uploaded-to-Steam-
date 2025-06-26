#include "InteractableKey.h"
#include <Menu/MySaveGame.h>
#include <Kismet/GameplayStatics.h>

AInteractableKey::AInteractableKey()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractableKey::BeginPlay()
{
	Super::BeginPlay();
	UMySaveGame* LoadGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("KeySaveSlot"), 0));
	if (LoadGame)
	{
		if (LoadGame->SavedKeys.Contains(GetFName()))
		{
			OnKeyPickedUp.Broadcast(this);
			Destroy();
		}
	}
	
}

void AInteractableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableKey::Interact()
{
	IsHaveKey = true;
	Destroy();
	OnKeyPickedUp.Broadcast(this);
	UMySaveGame* SaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (SaveGame)
	{
		SaveGame->SavedKeys.Add(GetFName(), IsHaveKey);
		UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("KeySaveSlot"), 0);
	}
}


