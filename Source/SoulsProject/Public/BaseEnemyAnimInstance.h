#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"


UCLASS()
class SOULSPROJECT_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	class ABaseEnemy* BaseEnemyCharacter;

	UPROPERTY(BlueprintReadOnly)
	bool IsMoving;

	UPROPERTY(BlueprintReadOnly)
	bool IsDeath;
	
	
};
