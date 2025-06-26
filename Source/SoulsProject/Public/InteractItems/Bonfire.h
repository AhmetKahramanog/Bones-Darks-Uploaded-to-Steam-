#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "../Weapons/BaseWeapon.h"
#include "Bonfire.generated.h"

enum class ECharacterStat : int8
{
	Health,
	Stamina,
	Strength,
};

UCLASS()
class SOULSPROJECT_API ABonfire : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	ABonfire();

	TMap<FName, ECharacterStat> ButtonNameToStatMap;

	void SetupStatButtons();

	void BindStatButton(FName ButtonName,ECharacterStat Stat);

	UFUNCTION()
	void OnStatButtonClicked();

	TMap<class UButton*, ECharacterStat> ButtonToStatMap;

	class UButton* StatButton;

	UFUNCTION()
	void AxePlus();

	UFUNCTION()
	void SpearPlus();

	UFUNCTION()
	void SwordPlus();

	virtual bool HasInteracted() override { return IsMenuOpen; }


	UPROPERTY(EditAnywhere, Category = "Health")
	int32 HealthIncreaseSoulAmount;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	int32 StaminaIncreaseSoulAmount;

	UPROPERTY(EditAnywhere, Category = "SoulAmount")
	int32 PriceSoulAmountIncrease;

	int32 StatSoulCost;

	UPROPERTY(EditAnywhere, Category = "Health")
	float PlayerMaxHealthIncrease;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float PlayerMaxStaminaIncrease;

	UPROPERTY(EditAnywhere, Category = "Strength")
	int32 IncreaseStrength;

	UPROPERTY(EditAnywhere, Category = "Strength")
	int32 StrengthSoulAmount;

	UPROPERTY(EditAnywhere, Category = "Strength")
	float IncreaseGiveWeaponStunnedTimer;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	int32 AxeWeaponSoulCost;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	int32 SpearWeaponSoulCost;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	float AxeWeaponDamageIncrease;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	float SpearWeaponDamageIncrease;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	int32 AxeSoulCostIncrease;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	int32 SpearSoulCostIncrease;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	int32 SwordSoulCost;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	int32 SwordSoulCostIncrease;

	UPROPERTY(EditAnywhere, Category = "WeaponPlus")
	float SwordWeaponDamageIncrease;

	

	class UImage* WeaponImage;

	class UWidgetSwitcher* BonfireMenuWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> UpgradeButtonsName;

	class UUserWidget* StatUpgradeMenuWidget;

	class UUserWidget* WeaponUpgradeMenuWidget;

	class UScrollBox* WeaponSlotList;

	void CreateSlotWeapon();

	void OnPlayerRest();

	UPROPERTY(EditAnywhere, Category = "Amount")
	int32 HealingPotAmount;

	UPROPERTY(EditAnywhere, Category = "Amount")
	float ReloadHealthAmountIncrease;

	int32 HealingPotIncreaseCount;

	void UpdatePotShardsAmount();
 
protected:
	virtual void BeginPlay() override;

	void UIController();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> BonfireMenuWidgetClass;

	void HealthButton();
	void StaminaButton();
	void StrengthButton();

	UPROPERTY()
	class UUserWidget* BonfireMenuWidget;

	class APlayerController* PlayerController;

	bool IsMenuOpen;

	void HandleStatButtons(class UProgressBar* ProgressBar,float& MaxStat,float& PlayerStat);

	void StatUpdateText();

	class UTextBlock* AxeWeaponSoulCostText;

	class UTextBlock* SpearWeaponSoulCostText;

	class UTextBlock* SwordWeaponSoulCostText;

	UFUNCTION()
	void HealingPotAmountIncrease();

	class UTextBlock* PotShardsText;

	class UTextBlock* PotShardsCostText;


private:
	class AMyCharacter* PlayerCharacter;

	int32 PlayerSoulAmount;

	int32 HealStatCount;

	int32 StaminaStatCount;

	class UButton* StatUpgradeButton;

	class UButton* WeaponUpgradeButton;


	class UButton* HealingPotIncreaseButton;

	UFUNCTION()
	void WeaponUpgradeFunc();

	UFUNCTION()
	void StatUpgradeFunc();

	void UpgradeWeaponSystem(EWeaponType WeaponType,float DamageAmount,int32& WeaponSoulAmount,int32 WeaponSoulCostIncrease,class UTextBlock* WeaponSoulCostText);

	class UTextBlock* HealthValueText;

	class UTextBlock* StaminaValueText;

	class UTextBlock* StrengthValueText;
			
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

};
