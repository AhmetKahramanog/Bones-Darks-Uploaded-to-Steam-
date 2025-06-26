#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.h"
#include "EnemyCharacterAnimInstance.generated.h"

UCLASS()
class SOULSPROJECT_API UEnemyCharacterAnimInstance : public UBaseEnemyAnimInstance
{
	GENERATED_BODY()

protected:
	class AEnemyCharacter* EnemyCharacter;

	class UCharacterMovementComponent* EnemyCharacterMC;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsMove;


	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	float Speed;
};
