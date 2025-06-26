#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class SOULSPROJECT_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	AArrow();

protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ArrowHitBox;

	UFUNCTION()
	void ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	class AEnemyRangedCharacter* EnemyRanged;

	FTimerHandle ArrowDestroyHandle;

public:	

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float ArrowDamage;


	virtual void Tick(float DeltaTime) override;

};
