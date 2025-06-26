#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/BaseWeapon.h"
#include "MyCharacterAnimInstance.generated.h"


UCLASS()
class SOULSPROJECT_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	class AMyCharacter* PlayerCharacter;
	
	class UCharacterMovementComponent* PlayerMovementC;

public:
	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	FVector DodgeVelocity;

	UPROPERTY(BlueprintReadOnly)
	bool CanDodge;

	UPROPERTY(BlueprintReadOnly)
	FVector Direction;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly)
	bool IsHealing;

	UPROPERTY(BlueprintReadOnly)
	EWeaponType WeaponTypeAnim;

	UPROPERTY(BlueprintReadOnly)
	bool IsPlayerDeath;

	
};
