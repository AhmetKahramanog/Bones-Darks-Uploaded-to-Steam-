#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class SOULSPROJECT_API AEnemyCharacter : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AEnemyCharacter();


	void MeleeAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* MeleeAttackMontage;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ForceReaction;*/

	

	/*UPROPERTY(EditAnyWhere,Category = "Combat")
	class UNiagaraSystem* BloodNiagaraEffect;*/

	virtual void EnemyTakeDamage(float amount,float StunnedTimer) override;


protected:
	virtual void BeginPlay() override;

	void Movement(float DeltaTime);

	class UAnimInstance* EnemyAnimInstance;

	/*void Stunned();*/

	/*bool IsStunned;*/

	/*void BloodEffectSpawn();*/

	/*class UNiagaraComponent* BloodEffect;*/

	void StopAttackMontage();

	virtual void EnemyAttackRaycast() override;

	virtual void Respawn() override;


private:
	FTimerHandle MeleeAttackTimerHandle;

	//FVector TargetLocationRandom;

	bool IsAttacking;

	FTimerHandle StopAttackHandle;

	//bool RandomIsMoving;

	//FTimerHandle StopRandomMovementHandle;

	bool CanBloodEffect;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
