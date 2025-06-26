#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.h"
#include "AxeWeapon.generated.h"

UCLASS()
class SOULSPROJECT_API AAxeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	AAxeWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
