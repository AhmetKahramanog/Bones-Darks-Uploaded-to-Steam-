#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDeathGate.generated.h"

UCLASS()
class SOULSPROJECT_API ABossDeathGate : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossDeathGate();

	UPROPERTY(EditAnywhere, Category = "Boss")
	class ABaseEnemy* Boss;

protected:
	virtual void BeginPlay() override;

private:
	FVector InitilazeLocation;

public:	
	virtual void Tick(float DeltaTime) override;

};
