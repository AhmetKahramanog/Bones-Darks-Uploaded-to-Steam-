#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BaseEnemyAnimNotify.generated.h"
UENUM(BlueprintType)
enum class EENemyType : uint8
{
	Kraken,
	Zombie,
};

UENUM(BlueprintType)
enum class EKrakenAttackState : uint8
{
	OpenCollider,
	CloseCollider,
	Movable,
	NotMovable
};

UCLASS()
class SOULSPROJECT_API UBaseEnemyAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void MeleeEnemyAttack();

	void KrakenBossAttack();

	UPROPERTY(EditAnyWhere)
	EENemyType EnemyType;

	UPROPERTY(EditAnyWhere)
	EKrakenAttackState KrakenAttackState;

private:
	class ABaseEnemy* EnemyCharacter;

	void KrakenCloseCollider();

	void BossMovingTimer();

	void BossMoving();

	void StopMoving();

	FTimerHandle MovingHandle;
	
};
