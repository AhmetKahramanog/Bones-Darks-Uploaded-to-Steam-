#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/BaseWeapon.h"
#include "Sword.generated.h"

UCLASS()
class SOULSPROJECT_API ASword : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	ASword();



protected:
	virtual void BeginPlay() override;
	

public:
	virtual void Tick(float DeltaTime) override;

};
