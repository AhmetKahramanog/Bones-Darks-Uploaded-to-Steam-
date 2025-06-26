#include "Menu/MySaveManager.h"
#include <Menu/MySaveGame.h>
#include <Kismet/GameplayStatics.h>
#include "MyCharacter.h"
#include "BaseEnemy.h"
#include "Weapons/BaseWeapon.h"
#include "InteractItems/Bonfire.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/ProgressBar.h>



void UMySaveManager::SaveGame(AMyCharacter* PlayerCharacter)
{
	UMySaveGame* SaveGameObject = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	
	if (!PlayerCharacter) { return; }

	if (SaveGameObject)
	{
		SaveGameObject->PlayerLocation = PlayerCharacter->GetActorLocation();
		SaveGameObject->SavedLastBonfireLocation = PlayerCharacter->LastBonfireLocation;
		SaveGameObject->PlayerHealth = PlayerCharacter->TargetHealth;
		SaveGameObject->SavedWeaponsId = PlayerCharacter->PlayerWeaponsID;
		SaveGameObject->SavedHealthPotion = PlayerCharacter->CurrentHealingPotNumber;
		SaveGameObject->SavedMaxHealthPotion = PlayerCharacter->MaxHealingPotNumber;
		SaveGameObject->SavedShardsAmountIncrease = PlayerCharacter->ShardsAmountIncrease;
		SaveGameObject->SavedHealingPotCost = PlayerCharacter->HealingPotShardsAmountCost;
		SaveGameObject->SavedMaxHealth = PlayerCharacter->MaxHealth;
		SaveGameObject->SavedMaxStamina = PlayerCharacter->MaxStamina;
		SaveGameObject->SavedSoulsAmount = PlayerCharacter->SoulCount;
		SaveGameObject->SavedShardsAmount = PlayerCharacter->HealingPotShardsAmount;
		SaveGameObject->SavedStatSoulCost = PlayerCharacter->PlayerStatSoulCost;
		SaveGameObject->SavedSpearWeaponSoulCost = PlayerCharacter->SpearWeaponSoulCost;
		SaveGameObject->SavedAxeWeaponSoulCost = PlayerCharacter->AxeWeaponSoulCost;
		SaveGameObject->SavedSwordWeaponSoulCost = PlayerCharacter->SwordWeaponSoulCost;
		SaveGameObject->SavedReloadHealth = PlayerCharacter->ReloadHealthAmount;
		UCanvasPanelSlot* HealthBarSlot = Cast<UCanvasPanelSlot>(PlayerCharacter->HealthBarProgress->Slot);
		if (HealthBarSlot)
		{
			SaveGameObject->SavedHealthBarSize = HealthBarSlot->GetSize();
		}
		UCanvasPanelSlot* StaminaBarSlot = Cast<UCanvasPanelSlot>(PlayerCharacter->StaminaBarProgress->Slot);
		if (StaminaBarSlot)
		{
			SaveGameObject->SavedStaminaBarSize = StaminaBarSlot->GetSize();
		}
		for (ABaseWeapon* Weapon : PlayerCharacter->WeaponsInventory)
		{
			if (Weapon)
			{
				SaveGameObject->SavedStrength = Weapon->AttackDamage;
				SaveGameObject->SavedWeaponStunned = Weapon->TakeStunnedTimer;
				SaveGameObject->SavedWeapons.Add(Weapon->GetClass());
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGameObject, TEXT("PlayerSaveSlot"), 0);
	}
}

void UMySaveManager::LoadGame(AMyCharacter* PlayerCharacter)
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"),0))
	{
		UMySaveGame* LoadGameObject = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"),0));
		if (!PlayerCharacter) { return; }

		if (LoadGameObject)
		{
			PlayerCharacter->SetActorLocation(LoadGameObject->PlayerLocation);
			PlayerCharacter->LastBonfireLocation = LoadGameObject->SavedLastBonfireLocation;
			PlayerCharacter->TargetHealth = LoadGameObject->PlayerHealth;
			PlayerCharacter->PlayerWeaponsID = LoadGameObject->SavedWeaponsId;
			PlayerCharacter->CurrentHealingPotNumber = LoadGameObject->SavedHealthPotion;
			PlayerCharacter->MaxHealingPotNumber = LoadGameObject->SavedMaxHealthPotion;
			PlayerCharacter->ShardsAmountIncrease = LoadGameObject->SavedShardsAmountIncrease;
			PlayerCharacter->HealingPotShardsAmountCost = LoadGameObject->SavedHealingPotCost;
			PlayerCharacter->MaxHealth = LoadGameObject->SavedMaxHealth;
			PlayerCharacter->MaxStamina = LoadGameObject->SavedMaxStamina;
			PlayerCharacter->SoulCount = LoadGameObject->SavedSoulsAmount;
			PlayerCharacter->HealingPotShardsAmount = LoadGameObject->SavedShardsAmount;
			PlayerCharacter->PlayerStatSoulCost = LoadGameObject->SavedStatSoulCost;
			PlayerCharacter->SpearWeaponSoulCost = LoadGameObject->SavedSpearWeaponSoulCost;
			PlayerCharacter->AxeWeaponSoulCost = LoadGameObject->SavedAxeWeaponSoulCost;
			PlayerCharacter->SwordWeaponSoulCost = LoadGameObject->SavedSwordWeaponSoulCost;
			PlayerCharacter->ReloadHealthAmount = LoadGameObject->SavedReloadHealth;
			UCanvasPanelSlot* HealthBarSlot = Cast<UCanvasPanelSlot>(PlayerCharacter->HealthBarProgress->Slot);
			if (HealthBarSlot)
			{
				HealthBarSlot->SetSize(LoadGameObject->SavedHealthBarSize);
			}
			UCanvasPanelSlot* StaminaBarSlot = Cast<UCanvasPanelSlot>(PlayerCharacter->StaminaBarProgress->Slot);
			if (StaminaBarSlot)
			{
				StaminaBarSlot->SetSize(LoadGameObject->SavedStaminaBarSize);
			}
			for (TSubclassOf<ABaseWeapon> WeaponClass : LoadGameObject->SavedWeapons)

			{
				ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
				if (NewWeapon)
				{
					NewWeapon->TakeStunnedTimer = LoadGameObject->SavedWeaponStunned;
					NewWeapon->AttackDamage = LoadGameObject->SavedStrength;
					PlayerCharacter->WeaponsInventory.Add(NewWeapon);
					NewWeapon->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
		}
	}
}

void UMySaveManager::ResetGame()
{
	UGameplayStatics::DeleteGameInSlot(TEXT("PlayerSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("BossDeathSaveSlot"),0);
	UGameplayStatics::DeleteGameInSlot(TEXT("ChestSaveSlot"), 0);
	UGameplayStatics::DeleteGameInSlot(TEXT("KeySaveSlot"), 0);
}

void UMySaveManager::BossDeathSave(ABaseEnemy* BossActor)
{
	UMySaveGame* BossDeathSaveObject = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (!BossActor) { return; }
	if (BossDeathSaveObject)
	{
		BossDeathSaveObject->SavedDeadBosses.Add(BossActor->BossName, true);
		BossDeathSaveObject->SavedBossDeath = BossActor->IsBossDie;
		UGameplayStatics::SaveGameToSlot(BossDeathSaveObject,TEXT("BossDeathSaveSlot"),0);
	}
}


