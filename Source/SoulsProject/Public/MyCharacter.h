#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IInteractable.h"
#include "Weapons/BaseWeapon.h"
#include "MyCharacter.generated.h"


class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class SOULSPROJECT_API AMyCharacter : public ACharacter, public IIInteractable
{
	GENERATED_BODY()

public:
	AMyCharacter();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Input")
	UInputAction* CameraLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadHealthAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ReloadHealthAmount;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Combat")
	TArray<class UAnimMontage*> SpearAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<class UAnimMontage*> SwordAttackMontages;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<class UAnimMontage*> AxeAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> FadeDamageEffectWidgetClass;

	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category = "Combat")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> StaminaBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* GetHitMontage;

	UPROPERTY(EditAnywhere, Category = "Heal")
	UAnimMontage* PotDrinkMontage;

	bool IsDrinkPot;

	UPROPERTY(EditAnywhere, Category = "Heal")
	class UNiagaraSystem* HealingEffect;

	UPROPERTY(EditAnywhere, Category = "Heal")
	int32 MaxHealingPotNumber;

	UPROPERTY(EditAnywhere, Category = "Heal")
	int32 HealingPotShardsAmountCost;

	int32 SoulCount;

	virtual void GetSoulAmountFlex(int32 Amount,int32 ShardsAmount) override;

	virtual void TakeSoulAmount(int32 Amount) override;

	class UProgressBar* HealthBarProgress;

	class UProgressBar* StaminaBarProgress;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	void PlayerReseted();

	class ABaseWeapon* CurrentSword;

	UPROPERTY(EditAnywhere, Category = "Healing")
	UAnimMontage* PotHealingMontage;

	EWeaponType CharacterWeaponType;

	bool IsEquipped;

	TArray<class ABaseWeapon*> WeaponsInventory;

	UPROPERTY()
	class ABaseWeapon* CurrentlyWeapon;

	void OnlyWeaponSlotInput();

	void EquipWeapon(int32 WeaponSlotIndex);

	int32 WeaponIndex;

	class ABaseWeapon* ClonedWeapon;

	TArray<FString> PlayerWeaponsID;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DeathScreenWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameMenuWidgetClass;

	class UUserWidget* GameMenuWidget;

	UPROPERTY()
	class ABonfire* LastRestedBonfire;

	class UButton* RespawnCharacter;

	UFUNCTION()
	void RespawnAtLastBonfire();

	void Die();

	bool IsDie;

	UPROPERTY(EditAnywhere, Category = "SoulAmount")
	int32 PlayerStatSoulCost;

	UPROPERTY(EditAnywhere, Category = "SoulAmount")
	int32 AxeWeaponSoulCost;

	UPROPERTY(EditAnywhere, Category = "SoulAmount")
	int32 SpearWeaponSoulCost;

	UPROPERTY(EditAnywhere, Category = "SoulAmount")
	int32 SwordWeaponSoulCost;

	int32 HealingPotShardsAmount;

	class UTextBlock* ShardsAmountText;

	class UTextBlock* EarnedSoulText;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float DodgeStaminaCost;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* GetHitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* SpearSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* DodgeSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HealingSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* DeathSound;

	void PlayerSoundPlay(USoundBase* PlaySound);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* OpenMenuAction;

	bool IsOpenGameInMenu;

	float CurrentHealht;

	float TargetHealth;

	class ABaseEnemy* TakenWeapon;

	TArray<FName> PlayerWeaponNames;

	int32 CurrentHealingPotNumber;

	FVector LastBonfireLocation;

	UPROPERTY(EditAnywhere, Category = "Amount")
	TSubclassOf<class ADeathSouls> DeathSoulClass;

	UPROPERTY()
	bool AlreadyDroppedSouls;

	int32 InitialSoulsAmount;

	UPROPERTY()
	class ADeathSouls* LastSouls;

	int32 ShardsAmountIncrease;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StartSprint();

	void StopSprint();

	UPROPERTY(EditDefaultsOnly)
	float DodgeForce;

	UPROPERTY(EditDefaultsOnly)
	float DodgeCoolDown;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* HealEffectComponent;


	bool CanDodge;

	void Dodge();

	void DodgeTiming(float DeltaTime);

	void InteractSword();

	void CameraLock();

	void CameraFocusedToTarget(float DeltaTime);

	class ABaseEnemy* ReturnedEnemy();

	void Attack();

	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	

	void AttackDamageDelay();

	void UpdateHealthBar(float DeltaTime);

	class UUserWidget* FadeDamageEffectWidget;

	class UImage* FadeDamageEffectImg;

	void UpdateFadeOutDamageEffect(float DeltaTime);

	void UpdateStaminaBar(float DeltaTime);

	void Stamina(float StaminaCost);

	bool IsStunned;

	FTimerHandle StunHandle;

	void CameraMiddleLineTrace();

	FHitResult CameraTraceHit;

	void GetWeaponSocket(FName SocketName,int32 Index);

	TMap<FKey, int32> EquippedWeaponsNumber;

	void AllWeaponsAttackHandle(TArray<class UAnimMontage*> AttackMontages,float SpendStaminaAmount,float AttackAnimationSpeed,USoundBase* WeaponSound);

	void CharacterForceInAttack();

	bool IsHeavyAttack;

	UPROPERTY(EditDefaultsOnly)
	float TwoHandAttackForceInterp;

	UPROPERTY(EditDefaultsOnly)
	float TwoHandAttackForceSpeed;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PickUpWeaponAction;

	void PickUpWeaponInput();


	void OpenGameMenu();

	UFUNCTION()
	void ReturnMainMenu();

private:
	
	bool IsCameraLock;
	int32 AttackIndex;
	bool CanAttack;
	

	FTimerHandle AttackDamageDelayHandle;

	

	FTimerHandle FadeTimerHandle;

	bool bIsFading;

	float TimeCounter;

	float CurrentStamina;

	bool CanReloadStamina;

	FTimerHandle StaminaReloadHandle;

	bool IsRunning;

	class UUserWidget* StaminaBarWidget;

	

	class UAnimInstance* AnimInstance;

	void ReloadHealth();

	FTimerHandle DrinkPotHandle;

	

	class UTextBlock* HealingAmountText;
	
	void HealingPotionNumber();

	class UTextBlock* SoulAmountText;

	IIInteractable* InteractObject;

	class UUserWidget* DeathScreenWidget;

	class UTextBlock* InteractText;

	class UButton* ReturnToMainMenuButton;

	

	APlayerController* PlayerController;

	FTimerHandle PlayerCharacterDeathHandle;

	FTimerHandle EarnedAmountTextHandle;

	int32 InitialPotShardsAmount;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetSword(class ABaseWeapon* Weapon);

	void GetDamage(float amount);

};
