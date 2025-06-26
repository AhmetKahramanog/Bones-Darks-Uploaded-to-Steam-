#include "InteractItems/InteractChest.h"
#include "MyCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Menu/MySaveGame.h>

AInteractChest::AInteractChest()
{
 	PrimaryActorTick.bCanEverTick = true;

	Chest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest"));
	Chest->SetupAttachment(RootComponent);

	ChestCover = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestCover"));
	ChestCover->SetupAttachment(Chest);

}

void AInteractChest::BeginPlay()
{
	Super::BeginPlay();

	UMySaveGame* LoadGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ChestSaveSlot"), 0));
	if (LoadGame)
	{
		if (LoadGame->SavedChestsMap.Contains(GetFName()))
		{
			IsOpen = LoadGame->SavedChestsMap[GetFName()];
			CanLoot = IsOpen;
		}
	}

	
}

void AInteractChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		ChestLocation = ChestCover->GetComponentLocation();
		CurrentRotationX = FRotator(0.f, 0.f, -40.f);
		FRotator ChestCoverOpen = FMath::RInterpTo(ChestCover->GetRelativeRotation(), CurrentRotationX, DeltaTime, 1.f);
		ChestCover->SetRelativeRotation(ChestCoverOpen);
	}

}

void AInteractChest::Interact()
{
	IsOpen = true;
	
	if (AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		IIInteractable* SoulTaker = Cast<IIInteractable>(PlayerActor);
		if (SoulTaker && !CanLoot)
		{
			SoulTaker->GetSoulAmountFlex(SoulAmount, PotShardsAmount);
			PotShardsAmount = 0;
			SoulAmount = 0;
			CanLoot = true;
		}
	}

	UMySaveGame* SaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ChestSaveSlot"),0));
	if (!SaveGame)
	{
		SaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	}
	if (SaveGame)
	{
		SaveGame->SavedChestsMap.Add(GetFName(), IsOpen);
		UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("ChestSaveSlot"), 0);
	}
}






