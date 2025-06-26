#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"


UCLASS()
class SOULSPROJECT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float Health;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UWidgetComponent* EnemyHealthBarWidgetComponent;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float LookSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveSpeed;

	virtual void EnemyTakeDamage(float amount,float StunnedTimer);

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FollowRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName ProgressHealthBarName;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FollowSpeed;

	class UProgressBar* HealthBar;
	class UUserWidget* EnemyHealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float CameraTargetRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FVector BoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float EnemyAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FRotator LookDirectionValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float EnableHudRange;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AttackIntervalRange;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	class UNiagaraSystem* BloodNiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float ForceReaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* HitReactionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* EnemyDeathMontage;

	bool IsStunned;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float StunnedTimer;*/

	UPROPERTY(EditAnywhere, Category = "Amount")
	int32 SoulAmount;

	virtual void Respawn();

	void Die();

	bool IsDie;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackAnimationSpeed;

	void SetNewRandomTarget();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RandomMoveTimer;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FollowToPlayerSpeed;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* EnemyHitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* EnemyAttackSound;

	void BossAttack();

	bool CanMove;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<class UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> BossHealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange;
	
	void BossHealthBarActive(ESlateVisibility Visible);

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* HitBoxCollider;

	bool IsAttacking;

	TArray<AActor*> HitActors;

	bool IsBossDie;

	UPROPERTY(EditAnywhere, Category = "ID")
	FName BossName;

protected:
	virtual void BeginPlay() override;

	float Distance();

	void Look(float DeltaTime);

	class APawn* PlayerCharacter;

	float CurrentHealth;

	virtual void EnemyAttackRaycast();

	bool IsHit;

	FHitResult HitResult;

	
	UFUNCTION()
	void OnHitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BaseMovement(float DeltaTime,float Speed);

	FRotator LookDirection;

	virtual void HitReactionImpulse();

	virtual void Stunned(float StunnedTimer);

	virtual void BloodEffectSpawn();

	FVector SpawnBaseEnemyLocation;
	FRotator SpawnBaseEnemyRotation;

	FTimerHandle DeathTimerHandle;

	FVector GetRandomLocation();

	class UAnimInstance* EnemyAnimInstance;

	FTimerHandle AttackHandle;

	class UUserWidget* BossHealthBarWidget;

	class UProgressBar* BossHealthBarProgress;

	void BossHealthBarEffect(float amount);

	void BossDeath();

	void ReturnDefaultHeatlhBar();

	FTimerHandle BossDeathHandle;

	FLinearColor DefaultBossHealthBar;

	FTimerHandle ReturnDefaultHealthBarHandle;

	

	FVector TargetLocationRandom;

	void StopRandomMovement();

	FTimerHandle StopRandomMovementHandle;

	bool RandomIsMoving;
	
	FTimerHandle RandomMovementTimer;

	void RandomMovingAroundPlayer(float DeltaTime);

	int32 AttackIndex;

private:
	class UNiagaraComponent* BloodEffect;

	

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
