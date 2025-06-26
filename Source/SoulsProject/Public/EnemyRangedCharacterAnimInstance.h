#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.h"
#include "EnemyRangedCharacterAnimInstance.generated.h"


UCLASS()
class SOULSPROJECT_API UEnemyRangedCharacterAnimInstance : public UBaseEnemyAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FVector UpperBodyDirection;

	UPROPERTY(BlueprintReadOnly)
	float AlphaValue;

protected:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
};
