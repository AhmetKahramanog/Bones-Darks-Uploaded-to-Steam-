#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "InteractableKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyPickedUp,AInteractableKey*, Key);


UCLASS()
class SOULSPROJECT_API AInteractableKey : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractableKey();

	FOnKeyPickedUp OnKeyPickedUp;

	UPROPERTY()
	bool IsHaveKey;

protected:
	virtual void BeginPlay() override;

	

	UPROPERTY(EditAnywhere, Category = "InteractableObjects")
	AActor* BossGate;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

};
