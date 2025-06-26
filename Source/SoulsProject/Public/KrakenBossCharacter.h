#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "KrakenBossCharacter.generated.h"



UCLASS()
class SOULSPROJECT_API AKrakenBossCharacter : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AKrakenBossCharacter();

	/*UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<class UAnimMontage*> AttackMontages;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> KrakenHealthBarWidgetClass;

	/*UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UBoxComponent* HitBoxCollider;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float KrakenAttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* KrakenGetHitMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* KrakenDeathMontage;

	/*void KrakenBossHealthBarActive(ESlateVisibility Visible);*/

	/*TArray<AActor*> HitActors;*/

	/*bool IsAttacking;*/

	virtual void Respawn() override;

protected:
	virtual void BeginPlay() override;

	void Movement(float DeltaTime);
	void Attack();

	virtual void EnemyTakeDamage(float amount,float StunnedTimer) override;

	/*class UAnimInstance* EnemyAnimInstance;*/

	void KrakenAttackRaycast();

	void KrakenAttackOnMontageEnded(UAnimMontage* AnimMontage,bool bInterrupted);

	/*UFUNCTION()
	void OnHitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/


private:
	class APawn* PlayerCharacter;
	/*int32 AttackIndex;*/
	class UUserWidget* KrakenHealthBarWidget;
	class UProgressBar* KrakenHealthProgressBar;
	FTimerHandle KrakenAttackDelayHandle;
	float KrakenTakeHitInterval;
	
	bool IsKrakenTakeDamage;

	void AttackTimerReset();

	bool IsKrakenDie;

	/*void KrakenDeath();*/

	/*FTimerHandle KrakenBossDeathHandle;

	FLinearColor DefaultKrakenHealthBar;

	FTimerHandle ReturnDefaultHealthBarHandle;*/

	/*void ReturnDefaultHealthBar();*/


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
