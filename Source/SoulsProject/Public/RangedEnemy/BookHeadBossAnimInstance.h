#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../BaseEnemyAnimInstance.h"
#include "BookHeadBossAnimInstance.generated.h"

UCLASS()
class SOULSPROJECT_API UBookHeadBossAnimInstance : public UBaseEnemyAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	float Speed;
	
};
