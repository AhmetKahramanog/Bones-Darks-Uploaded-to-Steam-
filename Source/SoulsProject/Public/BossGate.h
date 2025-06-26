#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "BossGate.generated.h"

UCLASS()
class SOULSPROJECT_API ABossGate : public AActor , public IIInteractable
{
	GENERATED_BODY()
	
public:	
	ABossGate();

protected:
	virtual void BeginPlay() override;
private:
	bool IsOpen;

	void MoveGate(float DeltaTime, FVector TargetLocation);

	FVector InitialLocation;

	FVector OpenLocation;

	FTimerHandle DestroyGateHandle;

	UPROPERTY(EditAnywhere)
	class ABaseEnemy* BossEnemy;

	UFUNCTION()
	void HandleBossGateOpen(class AInteractableKey* PickedKey);

	UPROPERTY(EditAnywhere)
	AActor* AssignedKey;

	bool CanOpenBossGate;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

	virtual bool HasInteracted() override { return IsOpen; }

};
