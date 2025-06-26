#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KrakenBossCharacterAnimInstance.generated.h"


UCLASS()
class SOULSPROJECT_API UKrakenBossCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	class AKrakenBossCharacter* KrakenCharacter;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsMoving;
	
};
