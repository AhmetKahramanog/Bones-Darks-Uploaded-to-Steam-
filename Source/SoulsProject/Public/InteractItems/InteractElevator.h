#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../IInteractable.h"
#include "InteractElevator.generated.h"

UCLASS()
class SOULSPROJECT_API AInteractElevator : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractElevator();

	virtual void Interact() override;

	bool CanMovingPlatform;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
