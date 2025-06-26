#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MyCharacterAnimNotify.generated.h"

UCLASS()
class SOULSPROJECT_API UMyCharacterAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* FootStepSound;
	
};
