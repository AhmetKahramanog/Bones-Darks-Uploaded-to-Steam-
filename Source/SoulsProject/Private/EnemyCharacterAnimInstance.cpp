#include "EnemyCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

void UEnemyCharacterAnimInstance::NativeInitializeAnimation()
{
	EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
	{
		EnemyCharacterMC = EnemyCharacter->GetCharacterMovement();
	}
}

void UEnemyCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (EnemyCharacter && EnemyCharacterMC)
	{
		Speed = EnemyCharacter->GetVelocity().Size2D();
		IsMove = Speed > 0.5f;
		IsDeath = EnemyCharacter->IsDie;
	}
}


