#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "EnemyRangedCharacter.generated.h"

UCLASS()
class SOULSPROJECT_API AEnemyRangedCharacter : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AEnemyRangedCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void BaseMovement(float DeltaTime,float Speed) override;

	void RangerLook(float DeltaTime);

	void RangedAttack();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AArrow> ArrowClass;

	void SpawnArrow();

	void FireArrow();

	bool IsFollowArrow;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ArrowForce;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ArrowSpawnSocket;

private:
	class UAnimInstance* AnimInstance;
	bool IsAttackIdle;
	FTimerHandle RangedAttackHandle;
	FTimerHandle RangedAttackInitialize;

	class AArrow* SpawnedArrow;

	FTimerHandle ArrowPhysicHandle;

	virtual void EnemyTakeDamage(float amount,float StunnedTimer) override;

	float AttackCounter;

	class UEnemyRangedCharacterAnimInstance* RangedAnimInstance;

public:	

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BowMesh;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* BeginAttack;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackIdle;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackEnd;

	void AttackIdleAnimation();

	void RangedEndAttack();

	virtual void Respawn() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
