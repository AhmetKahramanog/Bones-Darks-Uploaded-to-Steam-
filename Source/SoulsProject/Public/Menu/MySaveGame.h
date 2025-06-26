#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

UCLASS()
class SOULSPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FVector PlayerLocation;

	UPROPERTY()
	float PlayerHealth;

	UPROPERTY()
	TArray<class ABaseWeapon*> PlayerWeaponsInventory;

	UPROPERTY()
	bool IsPlayerTakeWeapon;

	UPROPERTY()
	TArray<FString> SavedWeaponsId;

	UPROPERTY()
	TArray<FName> SavedWeaponNames;

	UPROPERTY()
	TArray<TSubclassOf<ABaseWeapon>> SavedWeapons;

	UPROPERTY()
	TMap<FName, bool> SavedDeadBosses;

	UPROPERTY()
	bool SavedBossDeath;

	UPROPERTY()
	int32 SavedHealthPotion;

	UPROPERTY()
	int32 SavedMaxHealthPotion;

	UPROPERTY()
	int32 SavedShardsAmountIncrease;

	UPROPERTY()
	int32 SavedHealingPotCost;

	UPROPERTY()
	float SavedMaxHealth;

	UPROPERTY()
	float SavedMaxStamina;

	UPROPERTY()
	int32 SavedSoulsAmount;

	UPROPERTY()
	int32 SavedShardsAmount;

	UPROPERTY()
	TMap<FName, bool> SavedKeys;

	UPROPERTY()
	int32 SavedStatSoulCost;

	UPROPERTY()
	int32 SavedSwordWeaponSoulCost;

	UPROPERTY()
	int32 SavedAxeWeaponSoulCost;

	UPROPERTY()
	int32 SavedSpearWeaponSoulCost;

	UPROPERTY()
	FVector2D SavedHealthBarSize;

	UPROPERTY()
	FVector2D SavedStaminaBarSize;

	UPROPERTY()
	TMap<FName, bool> SavedChestsMap;

	UPROPERTY()
	float SavedStrength;

	UPROPERTY()
	FVector SavedLastBonfireLocation;

	UPROPERTY()
	float SavedWeaponStunned;

	UPROPERTY()
	float SavedReloadHealth;


};
