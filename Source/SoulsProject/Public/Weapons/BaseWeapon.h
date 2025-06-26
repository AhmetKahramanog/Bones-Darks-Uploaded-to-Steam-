#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Axe,
	Sword,
	Spear
};


UCLASS()
class SOULSPROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponName;

	UPROPERTY(EditAnywhere, Category = "Image")
	UTexture2D* WeaponIcon;

	UPROPERTY(EditAnywhere, Category = "ButtonName")
	FName WeaponButtonName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float TakeStunnedTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float SpendStaminaAttack;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitFleshSound;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;

	bool IsTakeWeapon;

	UPROPERTY(EditAnywhere, Category = "ID")
	FString WeaponID;

protected:
	virtual void BeginPlay() override;

	TArray<AActor*> HitActors;


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

public:	
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollider;

	

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* BloodEffectScene;

	void WeaponHitCollider(ECollisionEnabled::Type Collision, bool canHit);

	void SpawnBloodEffect();

	void DestroyBloodEffect();

private:
	class UNiagaraComponent* SpawnedBloodEffect;

	FTimerHandle DestroySpawnedBloodEffectHandle;

};
