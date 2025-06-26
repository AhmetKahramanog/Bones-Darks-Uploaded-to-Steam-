#include "RangedEnemy/RangedEnemyAnimNotify.h"
#include "EnemyRangedCharacter.h"


void URangedEnemyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) { return; }
	
	AEnemyRangedCharacter* EnemyRanged = Cast<AEnemyRangedCharacter>(MeshComp->GetOwner());
	if (EnemyRanged)
	{
		if (!EnemyRanged->IsStunned)
		{
			switch (NotifyType)
			{
			case ERangedEnemyNotifyType::IdleAim:
				EnemyRanged->AttackIdleAnimation();
				break;
			case ERangedEnemyNotifyType::EndAim:
				EnemyRanged->RangedEndAttack();
				break;
			default:
				break;
			}
		}
	}
}
