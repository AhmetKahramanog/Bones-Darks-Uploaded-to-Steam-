#include "KrakenBossCharacterAnimInstance.h"
#include "KrakenBossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UKrakenBossCharacterAnimInstance::NativeInitializeAnimation()
{
	KrakenCharacter = Cast<AKrakenBossCharacter>(TryGetPawnOwner());
}

void UKrakenBossCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (KrakenCharacter)
	{
		IsMoving = KrakenCharacter->GetVelocity().Size2D() > 0.1f;
	}
}
