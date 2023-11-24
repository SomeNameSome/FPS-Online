#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstancePrimary.generated.h"



UCLASS()
class FPBLANK_API UAnimInstancePrimary final : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	UAnimSequenceBase* WeaponBasePoseRef;

public:
	void SetNewBasePose(UAnimSequenceBase* NewPose);
};
