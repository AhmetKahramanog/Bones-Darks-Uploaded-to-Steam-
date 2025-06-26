#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../IInteractable.h"
#include "LockedDoor.generated.h"

UCLASS()
class SOULSPROJECT_API ALockedDoor : public AActor,public IIInteractable
{
	GENERATED_BODY()
	
public:	
	ALockedDoor();

	virtual void Interact() override;

	virtual bool HasInteracted() override { return IsOpenDoor; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Door")
	AActor* AssignedKey;

private:
	bool CanOpen;

	UFUNCTION()
	void OpenDoor(class AInteractableKey* Key);

	bool IsOpenDoor;

	FRotator TargetRotation;

	FRotator CurrentRotation;

	FTimerHandle DestroyDoorHandle;

public:	
	virtual void Tick(float DeltaTime) override;

};
