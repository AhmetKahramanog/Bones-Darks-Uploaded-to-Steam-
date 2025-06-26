#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class SOULSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyManager();

	void RegisterEnemy(class ABaseEnemy* Enemy);

	void SpawnAllEnemies();

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<class ABaseEnemy>> AllEnemies;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
