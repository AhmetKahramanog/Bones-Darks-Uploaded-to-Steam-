#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformGround.generated.h"

UCLASS()
class SOULSPROJECT_API APlatformGround : public AActor
{
	GENERATED_BODY()
	
public:	
	APlatformGround();

	UPROPERTY(EditAnywhere, Category = "Interacts")
	class AInteractElevator* Elevator;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* GroundMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;

	UPROPERTY()
	FVector InitialLocation;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginCollider(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

private:
	
	FTimerHandle MovingHandle;
	bool IsMoving;

};
