#include "InteractItems/Bonfire.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include "MyCharacter.h"
#include "Components/ProgressBar.h"
#include <Components/CanvasPanel.h>
#include "Components/CanvasPanelSlot.h"
#include "Weapons/BaseWeapon.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include <Components/HorizontalBox.h>
#include <Components/TextBlock.h>
#include <RangedEnemy/EnemyManager.h>


ABonfire::ABonfire()
{
	PrimaryActorTick.bCanEverTick = true;

}



void ABonfire::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Character)
	{
		PlayerCharacter = Cast<AMyCharacter>(Character);
	}

	IsMenuOpen = false;

	if (BonfireMenuWidgetClass)
	{
		BonfireMenuWidget = CreateWidget<UUserWidget>(GetWorld(), BonfireMenuWidgetClass);
		if (!BonfireMenuWidget) { return; }
		BonfireMenuWidgetSwitcher = Cast<UWidgetSwitcher>(BonfireMenuWidget->GetWidgetFromName("BonfireMenuWidgetSwitcher"));
		StatUpgradeMenuWidget = Cast<UUserWidget>(BonfireMenuWidget->GetWidgetFromName("BP_BonfireWidget"));
		WeaponUpgradeMenuWidget = Cast<UUserWidget>(BonfireMenuWidget->GetWidgetFromName("BP_WeaponUpgradeWidget"));
		PotShardsText = Cast<UTextBlock>(StatUpgradeMenuWidget->GetWidgetFromName("HealingPotShardsAmount"));
		PotShardsCostText = Cast<UTextBlock>(StatUpgradeMenuWidget->GetWidgetFromName("HealingPotPlusCost"));
		HealingPotIncreaseButton = Cast<UButton>(StatUpgradeMenuWidget->GetWidgetFromName("HealingPotPlus"));
		if (HealingPotIncreaseButton)
		{
			HealingPotIncreaseButton->OnClicked.AddDynamic(this, &ABonfire::HealingPotAmountIncrease);
		}
		if (WeaponUpgradeMenuWidget)
		{
			WeaponSlotList = Cast<UScrollBox>(WeaponUpgradeMenuWidget->GetWidgetFromName("WeaponUpgradeScrollBox"));
			HealthValueText = Cast<UTextBlock>(WeaponUpgradeMenuWidget->GetWidgetFromName("HealthValueText"));
			StaminaValueText = Cast<UTextBlock>(WeaponUpgradeMenuWidget->GetWidgetFromName("StaminaValueText"));
			StrengthValueText = Cast<UTextBlock>(WeaponUpgradeMenuWidget->GetWidgetFromName("StrengthValueText"));
		}
		StatUpgradeButton = Cast<UButton>(BonfireMenuWidget->GetWidgetFromName("StatUpgrade"));
		WeaponUpgradeButton = Cast<UButton>(BonfireMenuWidget->GetWidgetFromName("WeaponUpgrade"));
		if (StatUpgradeButton)
		{
			StatUpgradeButton->OnClicked.AddDynamic(this, &ABonfire::StatUpgradeFunc);
		}
		if (WeaponUpgradeButton)
		{
			WeaponUpgradeButton->OnClicked.AddDynamic(this, &ABonfire::WeaponUpgradeFunc);
		}
		
	}
}

void ABonfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABonfire::HealingPotAmountIncrease()
{
	if (PlayerCharacter)
	{
		if (PlayerCharacter->HealingPotShardsAmount >= PlayerCharacter->HealingPotShardsAmountCost)
		{
			PlayerCharacter->MaxHealingPotNumber += HealingPotAmount;
			PlayerCharacter->HealingPotShardsAmount -= PlayerCharacter->HealingPotShardsAmountCost;
			PlayerCharacter->ReloadHealthAmount += ReloadHealthAmountIncrease;
			if (PlayerCharacter->ShardsAmountIncrease >= 2)
			{
				PlayerCharacter->ShardsAmountIncrease = 0;
				PlayerCharacter->HealingPotShardsAmountCost++;
			}
			PlayerCharacter->ShardsAmountIncrease++;
			UpdatePotShardsAmount();
		}
	}
}
void ABonfire::UpdatePotShardsAmount()
{
	if (!PotShardsText || !PotShardsCostText) { return; }
	PotShardsText->SetText(FText::FromString(FString::FromInt(PlayerCharacter->HealingPotShardsAmount)));
	PotShardsCostText->SetText(FText::FromString(FString::FromInt(PlayerCharacter->HealingPotShardsAmountCost)));
}



void ABonfire::CreateSlotWeapon()
{
	if (WeaponUpgradeMenuWidget)
	{
		if (WeaponSlotList)
		{
			WeaponSlotList->ClearChildren();
			if (PlayerCharacter)
			{
				for (auto WeaponSlot : PlayerCharacter->WeaponsInventory)
				{
					UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(WeaponSlotList);

					UImage* Image = NewObject<UImage>(HorizontalBox);
					FSlateBrush Brush;
					Brush.SetResourceObject(WeaponSlot->WeaponIcon);
					Brush.ImageSize = FVector2D(64.f, 64.f);
					Image->SetBrush(Brush);

					UTextBlock* WeaponSoulCostText = NewObject<UTextBlock>(HorizontalBox);
					
					UButton* WeaponButton = NewObject<UButton>(HorizontalBox);
					if (WeaponSlot->WeaponType == EWeaponType::Axe)
					{
						AxeWeaponSoulCostText = WeaponSoulCostText;
						AxeWeaponSoulCostText->SetText(FText::FromString(FString::FromInt(PlayerCharacter->AxeWeaponSoulCost)));
						WeaponButton->OnClicked.AddDynamic(this, &ABonfire::AxePlus);
					}
					else if (WeaponSlot->WeaponType == EWeaponType::Sword)
					{
						SpearWeaponSoulCostText = WeaponSoulCostText;
						SpearWeaponSoulCostText->SetText(FText::FromString(FString::FromInt(PlayerCharacter->SpearWeaponSoulCost)));
						WeaponButton->OnClicked.AddDynamic(this, &ABonfire::SpearPlus);
					}
					else if (WeaponSlot->WeaponType == EWeaponType::Spear)
					{
						SwordWeaponSoulCostText = WeaponSoulCostText;
						SwordWeaponSoulCostText->SetText(FText::FromString(FString::FromInt(PlayerCharacter->SwordWeaponSoulCost)));
						WeaponButton->OnClicked.AddDynamic(this, &ABonfire::SwordPlus);
					}
					HorizontalBox->AddChildToHorizontalBox(Image);
					HorizontalBox->AddChildToHorizontalBox(WeaponSoulCostText);
					HorizontalBox->AddChildToHorizontalBox(WeaponButton);
					WeaponSlotList->AddChild(HorizontalBox);
				}
			}
		}
	}
}

void ABonfire::UpgradeWeaponSystem(EWeaponType WeaponType, float DamageAmount,int32& WeaponSoulAmount,int32 WeaponSoulCostIncrease,UTextBlock* WeaponSoulCostText)
{
	if (!PlayerCharacter) { return; }
	
	if (PlayerSoulAmount >= WeaponSoulAmount)
	{
		
		for (ABaseWeapon* Weapon : PlayerCharacter->WeaponsInventory)
		{
			if (Weapon)
			{
				if (Weapon->WeaponType == WeaponType)
				{
					Weapon->AttackDamage += DamageAmount;
					Weapon->TakeStunnedTimer += IncreaseGiveWeaponStunnedTimer;
					PlayerCharacter->TakeSoulAmount(WeaponSoulAmount);
					WeaponSoulAmount += WeaponSoulCostIncrease;
					PlayerSoulAmount = PlayerCharacter->SoulCount;
					WeaponSoulCostText->SetText(FText::FromString(FString::FromInt(WeaponSoulAmount)));
				}
			}
		}
	}
}

void ABonfire::AxePlus()
{
	UpgradeWeaponSystem(EWeaponType::Axe, AxeWeaponDamageIncrease, PlayerCharacter->AxeWeaponSoulCost,AxeSoulCostIncrease,AxeWeaponSoulCostText);
}

void ABonfire::SpearPlus()
{
	UpgradeWeaponSystem(EWeaponType::Sword, SpearWeaponDamageIncrease, PlayerCharacter->SpearWeaponSoulCost,SpearSoulCostIncrease,SpearWeaponSoulCostText);
}

void ABonfire::SwordPlus()
{
	UpgradeWeaponSystem(EWeaponType::Spear, SwordWeaponDamageIncrease, PlayerCharacter->SwordWeaponSoulCost, SwordSoulCostIncrease, SwordWeaponSoulCostText);
}

void ABonfire::WeaponUpgradeFunc()
{
	if (BonfireMenuWidgetSwitcher)
	{
		BonfireMenuWidgetSwitcher->SetActiveWidgetIndex(1);
		if (HealthValueText && StaminaValueText && StrengthValueText && PlayerCharacter && PlayerCharacter->CurrentlyWeapon)
		{
			FString HealthFormattedText = TEXT("Health: ") + FString::FromInt(PlayerCharacter->MaxHealth);
			HealthValueText->SetText(FText::FromString(HealthFormattedText));
			FString StaminaFormattedText = TEXT("Stamina: ") + FString::FromInt(PlayerCharacter->MaxStamina);
			StaminaValueText->SetText(FText::FromString(StaminaFormattedText));
			FString StrengthFormattedText = TEXT("Strength: ") + FString::FromInt(PlayerCharacter->CurrentlyWeapon->AttackDamage);
			StrengthValueText->SetText(FText::FromString(StrengthFormattedText));
		}
	}
}

void ABonfire::StatUpgradeFunc()
{
	if (BonfireMenuWidgetSwitcher)
	{
		BonfireMenuWidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void ABonfire::OnPlayerRest()
{
	AEnemyManager* EnemyManager = Cast<AEnemyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass()));
	if (EnemyManager)
	{
		EnemyManager->SpawnAllEnemies();
	}
}

void ABonfire::Interact()
{
	if (!BonfireMenuWidget && BonfireMenuWidgetClass)
	{
		BonfireMenuWidget = CreateWidget<UUserWidget>(GetWorld(), BonfireMenuWidgetClass);
	}
	IsMenuOpen = !IsMenuOpen;
	OnPlayerRest();
	UIController();
}

void ABonfire::SetupStatButtons()
{
	ButtonNameToStatMap = {
		{"HealthButton",ECharacterStat::Health},
		{"StaminaButton",ECharacterStat::Stamina},
		{"StrengthButton",ECharacterStat::Strength}
	};
	for (const auto& Elem : ButtonNameToStatMap)
	{
		BindStatButton(Elem.Key, Elem.Value);
	}
}

void ABonfire::BindStatButton(FName ButtonName, ECharacterStat Stat)
{
	if (!BonfireMenuWidget) { return; }
	if (!StatUpgradeMenuWidget) { return; }
	if (!WeaponUpgradeMenuWidget) { return; }

	StatButton = Cast<UButton>(StatUpgradeMenuWidget->GetWidgetFromName(ButtonName));
	if (StatButton)
	{
		StatButton->OnClicked.RemoveDynamic(this, &ABonfire::OnStatButtonClicked);
		StatButton->OnClicked.AddDynamic(this, &ABonfire::OnStatButtonClicked);
		ButtonToStatMap.Add(StatButton, Stat);
	}
}

void ABonfire::OnStatButtonClicked()
{
	
	for (const auto& Elem : ButtonToStatMap)
	{
		if (Elem.Key->HasKeyboardFocus())
		{
			ECharacterStat Stat = Elem.Value;
			switch (Stat)
			{
			case ECharacterStat::Health:
				HealthButton();
				break;
			case ECharacterStat::Stamina:
				StaminaButton();
				break;
			case ECharacterStat::Strength:
				StrengthButton();
				break;
			default:
				break;
			}
			break;
		}
	}
}



void ABonfire::HandleStatButtons(UProgressBar* ProgressBar, float& MaxStat,float& PlayerStat)
{
	if (PlayerCharacter)
	{
		if (ProgressBar)
		{
			UCanvasPanelSlot* Panel = Cast<UCanvasPanelSlot>(ProgressBar->Slot);
			if (Panel)
			{
				if (PlayerSoulAmount >= PlayerCharacter->PlayerStatSoulCost)
				{
					FVector2D IncreaseStatName = Panel->GetSize();
					PlayerStat += MaxStat;
					IncreaseStatName.X += MaxStat;
					PlayerCharacter->TakeSoulAmount(PlayerCharacter->PlayerStatSoulCost);
					PlayerCharacter->PlayerStatSoulCost += PriceSoulAmountIncrease;
					StatUpdateText();
					PlayerSoulAmount = PlayerCharacter->SoulCount;
					Panel->SetSize(IncreaseStatName);
					PlayerCharacter->PlayerReseted();
				}
			}
		}
	}
}

void ABonfire::StatUpdateText()
{
	TArray<FName> StatsCostName =
	{
		"DurabilityCostText",
		"EnduranceCostText",
		"StrengthCostText"
	};

	if (!StatUpgradeMenuWidget) { return; }

	for (FName Name : StatsCostName)
	{
		UTextBlock* SoulText = Cast<UTextBlock>(StatUpgradeMenuWidget->GetWidgetFromName(Name));
		if (SoulText)
		{
			SoulText->SetText(FText::FromString(FString::FromInt(PlayerCharacter->PlayerStatSoulCost)));
		}
	}
}



void ABonfire::HealthButton()
{
	if (PlayerCharacter)
	{
		HandleStatButtons(PlayerCharacter->HealthBarProgress, PlayerMaxHealthIncrease, PlayerCharacter->MaxHealth);
	}
}

void ABonfire::StaminaButton()
{
	if (PlayerCharacter)
	{
		HandleStatButtons(PlayerCharacter->StaminaBarProgress, PlayerMaxStaminaIncrease, PlayerCharacter->MaxStamina);
	}
}

void ABonfire::StrengthButton()
{
	if (PlayerCharacter)
	{
		if (PlayerCharacter->ClonedWeapon && PlayerSoulAmount >= PlayerCharacter->PlayerStatSoulCost)
		{
			PlayerCharacter->ClonedWeapon->AttackDamage += IncreaseStrength;
			PlayerCharacter->ClonedWeapon->TakeStunnedTimer += IncreaseGiveWeaponStunnedTimer;
			PlayerCharacter->TakeSoulAmount(PlayerCharacter->PlayerStatSoulCost);
			PlayerCharacter->PlayerStatSoulCost += PriceSoulAmountIncrease;
			StatUpdateText();
			PlayerSoulAmount = PlayerCharacter->SoulCount;
		}
	}
}

void ABonfire::UIController()
{
	if (!PlayerController || !PlayerCharacter || !BonfireMenuWidget || !BonfireMenuWidgetSwitcher ||!WeaponUpgradeMenuWidget || !StatUpgradeMenuWidget) { return; };

	APlayerController* PC = Cast<APlayerController>(PlayerController);
	if (!PC) { return; }
	if (BonfireMenuWidget && BonfireMenuWidgetSwitcher)
	{
		if (IsMenuOpen)
		{
			if (!BonfireMenuWidget->IsInViewport())
			{
				BonfireMenuWidget->AddToViewport();
			}
			else
			{
				BonfireMenuWidget->SetVisibility(ESlateVisibility::Visible);
			}
			FInputModeGameAndUI InputMode;
			/*PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(InputMode);*/
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
			PlayerCharacter->PlayerReseted();
			PlayerSoulAmount = PlayerCharacter->SoulCount;
			SetupStatButtons();
			StatUpdateText();
			CreateSlotWeapon();
			UpdatePotShardsAmount();
			PlayerCharacter->LastRestedBonfire = this;
		}
		else
		{
			BonfireMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
			/*PlayerController->SetInputMode(InputMode);*/
		}
	}
}
