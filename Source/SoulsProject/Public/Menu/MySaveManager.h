#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MySaveManager.generated.h"

UCLASS()
class SOULSPROJECT_API UMySaveManager : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SaveGame(class AMyCharacter* PlayerCharacter);

	UFUNCTION()
	void LoadGame(class AMyCharacter* PlayerCharacter);

	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void BossDeathSave(class ABaseEnemy* BossActor);

	
};
