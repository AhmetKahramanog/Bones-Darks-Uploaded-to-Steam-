#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../BaseEnemy.h"
#include "BookHeadBossCharacter.generated.h"

UCLASS()
class SOULSPROJECT_API ABookHeadBossCharacter : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ABookHeadBossCharacter();

	virtual void EnemyTakeDamage(float amount, float StunnedTimer) override;

	void JumpAttack(float DeltaTime);

	void JumpAttackAnim();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* JumpAttackMontage;

	virtual void Respawn() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float RunSpeed;

private:
	FTimerHandle JumpAttackHandle;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
