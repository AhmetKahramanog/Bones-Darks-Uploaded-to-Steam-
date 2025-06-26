#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenu.generated.h"

UCLASS()
class SOULSPROJECT_API AMainMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	AMainMenu();

	UPROPERTY(EditAnywhere,Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	class UUserWidget* MainMenuWidget;

protected:
	virtual void BeginPlay() override;

	class UWidgetSwitcher* MenuSwitcher;

	class UUserWidget* MenuWidget;

	class UUserWidget* MenuSettingsWidget;

	class UButton* SettingsButton;

	class UButton* ReturnMenuButton;

	class UButton* PlayButton;

	class UButton* RestartButton;

	class UButton* ExitButton;

	UFUNCTION()
	void Settings();

	UFUNCTION()
	void ReturnToMenu();

	UFUNCTION()
	void Play();

	UFUNCTION()
	void Restart();

	UFUNCTION()
	void Exit();

private:
	APlayerController* PC;

public:	
	virtual void Tick(float DeltaTime) override;

};
