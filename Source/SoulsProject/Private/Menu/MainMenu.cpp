#include "Menu/MainMenu.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include <Menu/MySaveManager.h>


AMainMenu::AMainMenu()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMainMenu::BeginPlay()
{
	Super::BeginPlay();

	PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		PC->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport(1);
			MenuSwitcher = Cast<UWidgetSwitcher>(MainMenuWidget->GetWidgetFromName("MainMenuWidgetSwitcher"));
			MenuWidget = Cast<UUserWidget>(MainMenuWidget->GetWidgetFromName("BP_MenuScreenWidget"));
			MenuSettingsWidget = Cast<UUserWidget>(MainMenuWidget->GetWidgetFromName("BP_MenuSettingsWidget"));
			if (MenuWidget)
			{
				SettingsButton = Cast<UButton>(MenuWidget->GetWidgetFromName("SettingsButton"));
				PlayButton = Cast<UButton>(MenuWidget->GetWidgetFromName("PlayButton"));
				RestartButton = Cast<UButton>(MenuWidget->GetWidgetFromName("RestartGameButton"));
				ExitButton = Cast<UButton>(MenuWidget->GetWidgetFromName("QuitGameButton"));
				PlayButton->OnClicked.AddDynamic(this, &AMainMenu::Play);
				SettingsButton->OnClicked.AddDynamic(this, &AMainMenu::Settings);
				RestartButton->OnClicked.AddDynamic(this, &AMainMenu::Restart);
				ExitButton->OnClicked.AddDynamic(this, &AMainMenu::Exit);
			}
			if (MenuSettingsWidget)
			{
				ReturnMenuButton = Cast<UButton>(MenuSettingsWidget->GetWidgetFromName("ReturnToMenuButton"));
				if (ReturnMenuButton)
				{
					ReturnMenuButton->OnClicked.AddDynamic(this, &AMainMenu::ReturnToMenu);
				}
			}
		}
	}
}

void AMainMenu::Settings()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(1);
	}
}

void AMainMenu::ReturnToMenu()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(0);
	}
}

void AMainMenu::Play()
{
	UGameplayStatics::OpenLevel(this, FName("Cave_Ruins_Demo"));
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

void AMainMenu::Restart()
{
	UMySaveManager* ResetGame = Cast<UMySaveManager>(UGameplayStatics::GetGameInstance(this));
	if (ResetGame)
	{
		ResetGame->ResetGame();
	}
	UGameplayStatics::OpenLevel(this, FName("Cave_Ruins_Demo"));
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

void AMainMenu::Exit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void AMainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

