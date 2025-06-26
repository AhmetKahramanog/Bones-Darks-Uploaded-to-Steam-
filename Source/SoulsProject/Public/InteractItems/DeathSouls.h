#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../IInteractable.h"
#include "DeathSouls.generated.h"

UCLASS()
class SOULSPROJECT_API ADeathSouls : public AActor , public IIInteractable
{
	GENERATED_BODY()
	
public:	
	ADeathSouls();

	virtual void Interact() override;

	UPROPERTY()
	int32 DroppedSoulsAmount;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
