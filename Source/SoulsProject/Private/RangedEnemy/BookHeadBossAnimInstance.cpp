#include "RangedEnemy/BookHeadBossAnimInstance.h"
#include "BaseEnemy.h"

void UBookHeadBossAnimInstance::NativeBeginPlay()
{

}

void UBookHeadBossAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (BaseEnemyCharacter)
	{
		Speed = BaseEnemyCharacter->GetVelocity().Size2D();
	}
}
