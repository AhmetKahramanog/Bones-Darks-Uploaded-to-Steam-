#include "BaseEnemyAnimInstance.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	BaseEnemyCharacter = Cast<ABaseEnemy>(TryGetPawnOwner());
}

void UBaseEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (BaseEnemyCharacter)
	{
		IsMoving = BaseEnemyCharacter->GetVelocity().Size2D() > 0.1f;
	}
}
