#include "MyCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include <MyCharacterAnimInstance.h>
#include <Kismet/KismetMathLibrary.h>
#include "Weapons/BaseWeapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "EngineUtils.h"
#include <Components/ProgressBar.h>
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include "BaseEnemy.h"
#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <Components/TextBlock.h>
#include <IInteractable.h>
#include <Components/CanvasPanelSlot.h>
#include <InteractItems/Bonfire.h>
#include "Components/SphereComponent.h"
#include <Components/Button.h>
#include "KrakenBossCharacter.h"
#include <Menu/MySaveManager.h>
#include "Components/WidgetSwitcher.h"
#include <Menu/MySaveGame.h>
#include <InteractItems/DeathSouls.h>



AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	HealEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HealingEffect"));
	HealEffectComponent->SetupAttachment(GetMesh());
	HealEffectComponent->bAutoActivate = false;

}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerReseted();

	AnimInstance = GetMesh()->GetAnimInstance();

	InitialPotShardsAmount = HealingPotShardsAmount;

	InitialSoulsAmount = SoulCount;

	

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (FadeDamageEffectWidgetClass)
	{
		FadeDamageEffectWidget = CreateWidget<UUserWidget>(GetWorld(), FadeDamageEffectWidgetClass);
		if (FadeDamageEffectWidget)
		{
			FadeDamageEffectWidget->AddToViewport();
		}
	}

	if (StaminaBarWidgetClass)
	{
		StaminaBarWidget = CreateWidget<UUserWidget>(GetWorld(), StaminaBarWidgetClass);
		if (StaminaBarWidget)
		{
			StaminaBarWidget->AddToViewport();
			StaminaBarWidget->SetVisibility(ESlateVisibility::Visible);
			HealthBarProgress = Cast<UProgressBar>(StaminaBarWidget->GetWidgetFromName("HealthBar"));
			StaminaBarProgress = Cast<UProgressBar>(StaminaBarWidget->GetWidgetFromName("Stamina_Bar"));
			HealingAmountText = Cast<UTextBlock>(StaminaBarWidget->GetWidgetFromName("HealingPotAmountText"));
			SoulAmountText = Cast<UTextBlock>(StaminaBarWidget->GetWidgetFromName("SoulAmountText"));
			InteractText = Cast<UTextBlock>(StaminaBarWidget->GetWidgetFromName("InteractText"));
			ShardsAmountText = Cast<UTextBlock>(StaminaBarWidget->GetWidgetFromName("ShardsAmountText"));
			EarnedSoulText = Cast<UTextBlock>(StaminaBarWidget->GetWidgetFromName("EarnedSoulsText"));
		}
	}

	if (DeathScreenWidgetClass)
	{
		DeathScreenWidget = CreateWidget<UUserWidget>(GetWorld(), DeathScreenWidgetClass);
		if (DeathScreenWidget)
		{
			RespawnCharacter = Cast<UButton>(DeathScreenWidget->GetWidgetFromName("BornAgain_Btn"));
			if (RespawnCharacter)
			{
				DeathScreenWidget->AddToViewport(1);
				RespawnCharacter->OnClicked.AddDynamic(this, &AMyCharacter::RespawnAtLastBonfire);
			}
		}
	}

	if (GameMenuWidgetClass)
	{
		GameMenuWidget = CreateWidget<UUserWidget>(GetWorld(), GameMenuWidgetClass);
		if (GameMenuWidget)
		{
			GameMenuWidget->AddToViewport(2);
			GameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			ReturnToMainMenuButton = Cast<UButton>(GameMenuWidget->GetWidgetFromName("GameInMenuButton"));
			if (ReturnToMainMenuButton)
			{
				ReturnToMainMenuButton->OnClicked.AddDynamic(this, &AMyCharacter::ReturnMainMenu);
			}
		}
	}


	UMySaveManager* LoadGame = Cast<UMySaveManager>(UGameplayStatics::GetGameInstance(this));
	if (LoadGame)
	{
		LoadGame->LoadGame(this);
	}

	if (HealingAmountText)
	{
		HealingPotionNumber();
	}
	if (SoulAmountText)
	{
		SoulAmountText->SetText(FText::FromString(FString::FromInt(SoulCount)));
	}
	
	CharacterWeaponType = EWeaponType::Sword;

	TimeCounter = 0;

}

void AMyCharacter::Tick(float DeltaTime)
{
	if (IsDie) { return; }

	Super::Tick(DeltaTime);

	CameraFocusedToTarget(DeltaTime);

	DodgeTiming(DeltaTime);

	UpdateHealthBar(DeltaTime);

	UpdateFadeOutDamageEffect(DeltaTime);

	UpdateStaminaBar(DeltaTime);

	if (IsRunning && CurrentStamina > 0.f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 450.f;
		IsRunning = false;
	}

	/*if (IsHeavyAttack)
	{
		CharacterForceInAttack(DeltaTime);
	}*/

	CameraMiddleLineTrace();
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementDirection = Value.Get<FVector2D>();

	if (Controller && CanDodge && CanAttack && !IsStunned)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementDirection.Y);
		AddMovementInput(RightDirection, MovementDirection.X);
	}
}

void AMyCharacter::RespawnAtLastBonfire()
{
	if (!PlayerController || !DeathScreenWidget || !FadeDamageEffectImg) { return; }
	if (LastRestedBonfire)
	{
		LastBonfireLocation = LastRestedBonfire->GetActorLocation() + FVector(-100.f, 200.f, 350.f);
		LastRestedBonfire->OnPlayerRest();
	}
	SetActorLocation(LastBonfireLocation);
	PlayerReseted();
	IsDie = false;
	PlayerController->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	EnableInput(PlayerController);
	DeathScreenWidget->SetVisibility(ESlateVisibility::Hidden);
	FadeDamageEffectImg->SetRenderOpacity(0);
}

void AMyCharacter::StartSprint()
{
	if (CurrentStamina > 0.f && !IsDrinkPot)
	{
		IsRunning = true;
	}
}

void AMyCharacter::StopSprint()
{
	IsRunning = false;
	Stamina(0.f);
}

void AMyCharacter::PlayerSoundPlay(USoundBase* PlaySound)
{
	if (!PlaySound) { return; }
	FVector Location = GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlaySound, Location);
}


void AMyCharacter::Look(const FInputActionValue& Value)
{

	if (Controller && !IsCameraLock)
	{
		FVector2D LookDirection = Value.Get<FVector2D>();
		AddControllerPitchInput(LookDirection.Y * -1);
		AddControllerYawInput(LookDirection.X);
	}
}

void AMyCharacter::CameraLock()
{

	if (ReturnedEnemy())
	{
		FVector EnemyDirection = (ReturnedEnemy()->GetActorLocation() - Camera->GetComponentLocation()).GetSafeNormal();
		float DotProduct = FVector::DotProduct(Camera->GetForwardVector(), EnemyDirection);
		float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
		if (Angle <= 60.f)
		{
			IsCameraLock = !IsCameraLock;
		}
	}
	else
	{
		IsCameraLock = false;
	}
}

void AMyCharacter::CameraFocusedToTarget(float DeltaTime)
{
	if (IsCameraLock)
	{
		FVector PlayerLocation = GetActorLocation();
		if (!ReturnedEnemy())
		{
			IsCameraLock = false;
			return;
		}
		FVector EnemyLocation = ReturnedEnemy()->GetActorLocation();
		FRotator TargetRotation = (EnemyLocation - PlayerLocation).Rotation();
		FRotator SmoothedRotation = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaTime, 5.f);
		SmoothedRotation.Pitch -= 2.f;
		GetController()->SetControlRotation(SmoothedRotation);
	}
}

ABaseEnemy* AMyCharacter::ReturnedEnemy()
{
	ABaseEnemy* Enemy = nullptr;

	for (TActorIterator<ABaseEnemy> It(GetWorld()); It; ++It)
	{
		Enemy = *It;
		if (Enemy)
		{
			if (Enemy->IsDie) { return nullptr; }
			float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
			if (Distance < Enemy->CameraTargetRange)
			{
				return Enemy;
			}
		}
	}
	return nullptr;
}

void AMyCharacter::CameraMiddleLineTrace()
{
	if (!InteractText) { return; }
	CameraTraceHit = FHitResult();
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 500.f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(CameraTraceHit, Start, End, ECollisionChannel::ECC_Camera, QueryParams);
	if (IsHit)
	{
		InteractObject = Cast<IIInteractable>(CameraTraceHit.GetActor());
		if (InteractObject && !InteractObject->HasInteracted())
		{
			InteractText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InteractText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		InteractObject = nullptr;
		InteractText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyCharacter::GetSoulAmountFlex(int32 Amount,int32 ShardsAmount)
{
	SoulCount += Amount;
	InitialSoulsAmount = SoulCount;
	HealingPotShardsAmount += ShardsAmount;
	if (SoulAmountText && EarnedSoulText && ShardsAmountText)
	{
		if (HealingPotShardsAmount > InitialPotShardsAmount)
		{
			ShardsAmountText->SetVisibility(ESlateVisibility::Visible);
			InitialPotShardsAmount = HealingPotShardsAmount;
		}
		EarnedSoulText->SetVisibility(ESlateVisibility::Visible);
		FString FormattedText = TEXT("x ") + FString::FromInt(Amount) + TEXT(" Souls");
		EarnedSoulText->SetText(FText::FromString(FormattedText));
		SoulAmountText->SetText(FText::FromString(FString::FromInt(SoulCount)));
		GetWorld()->GetTimerManager().SetTimer(EarnedAmountTextHandle, FTimerDelegate::CreateLambda([this]
			{
				if (ShardsAmountText && EarnedSoulText)
				{
					EarnedSoulText->SetVisibility(ESlateVisibility::Hidden);
					if (ShardsAmountText->IsVisible())
					{
						ShardsAmountText->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}), 3.5f, false);
	}
}

void AMyCharacter::TakeSoulAmount(int32 Amount)
{
	SoulCount -= Amount;
	if (SoulAmountText)
	{
		SoulAmountText->SetText(FText::FromString(FString::FromInt(SoulCount)));
	}
}


void AMyCharacter::DodgeTiming(float DeltaTime)
{
	if (!CanDodge)
	{
		if (IsStunned || GetCharacterMovement()->IsFalling()) { return; }
		FVector DodgeImpulse(GetActorForwardVector() * DodgeForce);
		GetCharacterMovement()->AddForce(DodgeImpulse);
	}
}


void AMyCharacter::Dodge()
{
	if (AnimInstance && CanDodge && CanAttack && CurrentStamina > 0.f && !IsStunned && !IsDrinkPot && !GetCharacterMovement()->IsFalling())
	{
		Stamina(DodgeStaminaCost);
		CanDodge = false;
		AnimInstance->Montage_Play(DodgeMontage,2.f);
		PlayerSoundPlay(DodgeSound);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]
			{
				CanDodge = true;
			}), DodgeCoolDown, false);
	}
}

void AMyCharacter::GetSword(ABaseWeapon* Weapon)
{
	CurrentSword = Weapon;
}

void AMyCharacter::Attack()
{
	if (!CurrentlyWeapon) { return; }

	switch (CharacterWeaponType)
	{
	case EWeaponType::Axe:
		AllWeaponsAttackHandle(AxeAttackMontages, CurrentlyWeapon->SpendStaminaAttack, 0.8f,SpearSound);
		//IsHeavyAttack = true;
		break;
	case EWeaponType::Sword:
		AllWeaponsAttackHandle(SpearAttackMontages, CurrentlyWeapon->SpendStaminaAttack,1.65f,SpearSound);
		break;
	case EWeaponType::Spear:
		AllWeaponsAttackHandle(SwordAttackMontages, CurrentlyWeapon->SpendStaminaAttack, 0.4f, SpearSound);
		break;
	default:
		break;
	}
}
void AMyCharacter::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (CurrentlyWeapon)
	{
		CurrentlyWeapon->WeaponHitCollider(ECollisionEnabled::NoCollision, false);
		CanAttack = true;
		IsHeavyAttack = false;
	}
}

void AMyCharacter::AttackDamageDelay()
{
	if (CurrentlyWeapon)
	{
		CurrentlyWeapon->WeaponHitCollider(ECollisionEnabled::QueryOnly, true);
	}
}

void AMyCharacter::AllWeaponsAttackHandle(TArray<UAnimMontage*> AttackMontages,float SpendStaminaAmount,float AttackAnimationSpeed,USoundBase* WeaponSound)
{
	if (AnimInstance && IsEquipped && CanAttack && CanDodge && CurrentStamina > 0.f && !IsStunned && !IsDrinkPot)
	{
		Stamina(SpendStaminaAmount);
		CanAttack = false;
		if (AttackIndex >= AttackMontages.Num())
		{
			AttackIndex = 0;
		}
		CharacterForceInAttack();
		PlayerSoundPlay(WeaponSound);
		GetWorldTimerManager().SetTimer(AttackDamageDelayHandle, this, &AMyCharacter::AttackDamageDelay, 0.3f, false);
		AnimInstance->Montage_Play(AttackMontages[AttackIndex], AttackAnimationSpeed);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMyCharacter::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontages[AttackIndex]);
		AttackIndex++;
	}
}

void AMyCharacter::CharacterForceInAttack()
{
	FVector ForwardForce = GetActorForwardVector() * TwoHandAttackForceInterp;
	LaunchCharacter(ForwardForce, true, false);
}

void AMyCharacter::GetDamage(float amount)
{
	if (IsDie) { return; }
	if (CurrentHealht > 0.f)
	{
		TargetHealth = FMath::Clamp(TargetHealth - amount, 0.f, MaxHealth);
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(GetHitMontage);
			PlayerSoundPlay(GetHitSound);
			IsStunned = true;
			GetWorld()->GetTimerManager().SetTimer(StunHandle, FTimerDelegate::CreateLambda([this]
				{
					IsStunned = false;
				}), 0.5f, false);
		}

		FadeDamageEffectImg = Cast<UImage>(FadeDamageEffectWidget->GetWidgetFromName("FadeDamageEffect"));
	}
}

void AMyCharacter::ReloadHealth()
{
	if (CurrentHealingPotNumber > 0 && CurrentStamina > 0)
	{
		if (HealEffectComponent)
		{
			HealEffectComponent->Deactivate();
		}
		AnimInstance->Montage_Play(PotHealingMontage, 1.4f);
		if (FadeDamageEffectImg)
		{
			FadeDamageEffectImg->SetOpacity(0.f);
		}
		IsDrinkPot = true;
		CurrentHealingPotNumber--;
		PlayerSoundPlay(HealingSound);
		HealingPotionNumber();
		if (HealEffectComponent)
		{
			HealEffectComponent->Activate();
		}
		GetWorld()->GetTimerManager().SetTimer(DrinkPotHandle, FTimerDelegate::CreateLambda([this]
			{
				IsDrinkPot = false;
				HealEffectComponent->Deactivate();
			}), 1.5f, false);
		TargetHealth += ReloadHealthAmount;
		if (TargetHealth >= MaxHealth)
		{
			TargetHealth = MaxHealth;
		}
	}
}

void AMyCharacter::HealingPotionNumber()
{
	if (HealingAmountText)
	{
		HealingAmountText->SetText(FText::FromString(FString::FromInt(CurrentHealingPotNumber)));
	}
}

void AMyCharacter::UpdateFadeOutDamageEffect(float DeltaTime)
{
	TimeCounter += DeltaTime;
	if (FadeDamageEffectImg && CurrentHealht < 30.f)
	{
		if (TimeCounter > 5.f)
		{
			FadeDamageEffectImg->SetRenderOpacity(0.8f);
			TimeCounter = 0;
		}
		float CurrentOpacity = FadeDamageEffectImg->GetRenderOpacity();
		float NewOpacity = FMath::FInterpTo(CurrentOpacity, 0.f, DeltaTime, 0.5f);
		FadeDamageEffectImg->SetRenderOpacity(NewOpacity);

	}
}

void AMyCharacter::Die()
{
	if (DeathScreenWidget && PlayerController)
	{
		if (LastSouls && LastSouls->IsValidLowLevel())
		{
			LastSouls->Destroy();
		}
		FActorSpawnParameters SpawnParameters;
		ADeathSouls* DroppedSouls = GetWorld()->SpawnActor<ADeathSouls>(DeathSoulClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
		if (DroppedSouls)
		{
			DroppedSouls->DroppedSoulsAmount = SoulCount;
			LastSouls = DroppedSouls;
		}
		DeathScreenWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		SoulCount = 0;
		if (SoulAmountText && EarnedSoulText)
		{
			SoulAmountText->SetText(FText::FromString(FString::FromInt(SoulCount)));
			EarnedSoulText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMyCharacter::PlayerReseted()
{
	CanDodge = true;
	CanAttack = true;
	CurrentHealingPotNumber = MaxHealingPotNumber;
	CurrentHealht = MaxHealth;
	TargetHealth = CurrentHealht;
	CurrentStamina = MaxStamina;
	HealingPotionNumber();
}

void AMyCharacter::UpdateHealthBar(float DeltaTime)
{
	if (HealthBarProgress && PlayerController)
	{
		if (TargetHealth > 0)
		{
			CurrentHealht = FMath::FInterpTo(CurrentHealht, TargetHealth, DeltaTime, 4.3f);
			HealthBarProgress->SetPercent(CurrentHealht / MaxHealth);
		}
		else
		{
			HealthBarProgress->SetPercent(0.f / MaxHealth);
			IsDie = true;
			
			PlayerSoundPlay(DeathSound);
			DisableInput(PlayerController);
			GetWorldTimerManager().SetTimer(PlayerCharacterDeathHandle,this ,&AMyCharacter::Die, 6.f, false);
		}
	}
}

void AMyCharacter::UpdateStaminaBar(float DeltaTime)
{
	if (CanReloadStamina && CurrentStamina <= MaxStamina && !IsRunning)
	{
		CurrentStamina += 0.7f;
	}
	else if (IsRunning && CurrentStamina >= 0.f)
	{
		if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) > 0.1f)
		{
			CurrentStamina -= 0.3f;
		}
	}
	if (StaminaBarProgress)
	{
		StaminaBarProgress->SetPercent(CurrentStamina / MaxStamina);
	}
	
}

void AMyCharacter::Stamina(float StaminaCost)
{
	if (CurrentStamina >= 0)
	{
		CurrentStamina -= StaminaCost;
	}
	CanReloadStamina = false;
	GetWorld()->GetTimerManager().SetTimer(StaminaReloadHandle, FTimerDelegate::CreateLambda([this]
		{
			CanReloadStamina = true;
		}), 2.5f, false);
}

void AMyCharacter::PickUpWeaponInput()
{
	if (CurrentSword && !WeaponsInventory.Contains(CurrentSword))
	{
		CurrentSword->IsTakeWeapon = true;
		ClonedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(CurrentSword->GetClass());
		if (!ClonedWeapon) { return; }
		WeaponsInventory.Add(ClonedWeapon);
		if (ClonedWeapon->WeaponMesh)
		{
			ClonedWeapon->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		PlayerWeaponsID.AddUnique(CurrentSword->WeaponID);
		CurrentSword->Destroy();
		CurrentSword = nullptr;
	}
}

void AMyCharacter::OpenGameMenu()
{
	if (!PlayerController || !GameMenuWidget) { return; }
	IsOpenGameInMenu = !IsOpenGameInMenu;

	if (IsOpenGameInMenu)
	{
		GameMenuWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		GameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

void AMyCharacter::ReturnMainMenu()
{
	if (PlayerController)
	{
		UMySaveManager* SaveManager = Cast<UMySaveManager>(UGameplayStatics::GetGameInstance(this));
		if (SaveManager)
		{
			SaveManager->SaveGame(this);
		}
		UGameplayStatics::OpenLevel(GetWorld(), FName("NewWorld"));
	}
}

void AMyCharacter::InteractSword()
{
	if (InteractObject)
	{
		InteractObject->Interact();
	}
}

void AMyCharacter::GetWeaponSocket(FName SocketName, int32 Index)
{
	if (IsValid(CurrentlyWeapon))
	{
		CurrentlyWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentlyWeapon->SetActorHiddenInGame(true);
	}
	CharacterWeaponType = WeaponsInventory[Index]->WeaponType;
	WeaponsInventory[Index]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	WeaponsInventory[Index]->SetActorHiddenInGame(false);
}

void AMyCharacter::EquipWeapon(int32 WeaponSlotIndex)
{
	if (WeaponsInventory.Num() > WeaponSlotIndex)
	{
		AttackIndex = 0;
		IsEquipped = true;
		WeaponIndex = WeaponSlotIndex;
		GetWeaponSocket(WeaponsInventory[WeaponIndex]->WeaponName, WeaponIndex);
		CurrentlyWeapon = WeaponsInventory[WeaponIndex];
	}
}

	
void AMyCharacter::OnlyWeaponSlotInput()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) { return; }
	if (WeaponsInventory.Num() <= 0) { return; }
	if (PC->IsInputKeyDown(EKeys::One))
	{
		EquipWeapon(0);
	}
	else if (PC->IsInputKeyDown(EKeys::Two))
	{
		EquipWeapon(1);
	}
	else if (PC->IsInputKeyDown(EKeys::Three))
	{
		EquipWeapon(2);
	}
}


void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AMyCharacter::Dodge);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyCharacter::InteractSword);
		EnhancedInput->BindAction(CameraLockAction, ETriggerEvent::Triggered, this, &AMyCharacter::CameraLock);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::Attack);
		EnhancedInput->BindAction(ReloadHealthAction, ETriggerEvent::Triggered, this, &AMyCharacter::ReloadHealth);
		EnhancedInput->BindAction(PickUpWeaponAction, ETriggerEvent::Triggered, this, &AMyCharacter::PickUpWeaponInput);
		EnhancedInput->BindAction(OpenMenuAction, ETriggerEvent::Triggered, this, &AMyCharacter::OpenGameMenu);
	}

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AMyCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AMyCharacter::StopSprint);
	PlayerInputComponent->BindAction("EquppedWeaponSlot", EInputEvent::IE_Pressed, this, &AMyCharacter::OnlyWeaponSlotInput);

}

