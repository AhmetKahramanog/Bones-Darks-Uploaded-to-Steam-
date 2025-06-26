#include "Sword.h"
#include "Components/SphereComponent.h"
#include <MyCharacter.h>
#include "Components/BoxComponent.h"
#include <EnemyCharacter.h>
#include <BaseEnemy.h>

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

