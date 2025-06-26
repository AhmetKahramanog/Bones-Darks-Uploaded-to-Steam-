#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

UINTERFACE(MinimalAPI)
class UIInteractable : public UInterface
{
	GENERATED_BODY()
};


class SOULSPROJECT_API IIInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact();

	virtual void GetSoulAmountFlex(int32 Amount,int32 ShardsAmount);

	virtual void TakeSoulAmount(int32 Amount);

	virtual bool HasInteracted();

};
