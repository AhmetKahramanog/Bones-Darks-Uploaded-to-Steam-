#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.h"
#include "LongSword.generated.h"

UCLASS()
class SOULSPROJECT_API ALongSword : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	ALongSword();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
