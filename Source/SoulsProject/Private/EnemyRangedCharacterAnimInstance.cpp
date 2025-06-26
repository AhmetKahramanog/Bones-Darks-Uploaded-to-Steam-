#include "EnemyRangedCharacterAnimInstance.h"
#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyRangedCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (BaseEnemyCharacter)
	{
		IsMoving = BaseEnemyCharacter->GetVelocity().Size() > 1.f;
		IsDeath = BaseEnemyCharacter->IsDie;
	}
}
