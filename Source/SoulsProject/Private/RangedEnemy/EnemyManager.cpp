#include "RangedEnemy/EnemyManager.h"
#include "BaseEnemy.h"
#include <KrakenBossCharacter.h>
#include <Components/SlateWrapperTypes.h>

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::RegisterEnemy(ABaseEnemy* Enemy)
{
	if (Enemy)
	{
		AllEnemies.Add(Enemy);
	}
}

void AEnemyManager::SpawnAllEnemies()
{
	for (TWeakObjectPtr<ABaseEnemy> EnemyPtr : AllEnemies)
	{
		if (EnemyPtr.IsValid())
		{
			EnemyPtr->Respawn();
			EnemyPtr->BossHealthBarActive(ESlateVisibility::Hidden);
		}
	}
}
