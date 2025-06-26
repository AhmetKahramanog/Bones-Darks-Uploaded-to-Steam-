
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RangedEnemyAnimNotify.generated.h"


UENUM(BlueprintType)
enum class ERangedEnemyNotifyType : uint8
{
	IdleAim UMETA(DisplayName = "Play Idle"),
	EndAim UMETA(DisplayName = "End Idle"),
};


UCLASS()
class SOULSPROJECT_API URangedEnemyAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	FName AnimationNotifyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	ERangedEnemyNotifyType NotifyType;

};
