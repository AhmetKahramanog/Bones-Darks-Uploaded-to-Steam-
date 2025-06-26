#include "MyCharacterAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "GameFramework/Character.h"


void UMyCharacterAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		PlayerMovementC = PlayerCharacter->GetCharacterMovement();
	}
}


void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (PlayerMovementC && PlayerCharacter)
	{
		Speed = UKismetMathLibrary::VSizeXY(PlayerMovementC->Velocity);
		DodgeVelocity = PlayerCharacter->GetCharacterMovement()->Velocity.GetSafeNormal2D();
		Direction = PlayerCharacter->GetActorLocation();
		IsFalling = PlayerMovementC->IsFalling();
		IsHealing = PlayerCharacter->IsDrinkPot;
		WeaponTypeAnim = PlayerCharacter->CharacterWeaponType;
		IsPlayerDeath = PlayerCharacter->IsDie;
	}
}
