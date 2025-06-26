#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IInteractable.h"
#include "InteractChest.generated.h"

UCLASS()
class SOULSPROJECT_API AInteractChest : public AActor , public IIInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractChest();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Chest;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ChestCover;

	UPROPERTY(EditAnywhere, Category = "Amount")
	int32 SoulAmount;

	UPROPERTY(EditAnyWhere,Category = "Amount")
	int32 PotShardsAmount;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

	virtual bool HasInteracted() override { return IsOpen; }

	UPROPERTY()
	bool CanLoot;

private:
	bool IsOpen;

	FVector ChestLocation;

	FRotator CurrentRotationX;

	

};
