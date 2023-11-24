#include "AnimInstancePrimary.h"



void UAnimInstancePrimary::SetNewBasePose(UAnimSequenceBase* NewPose)
{
	if (!NewPose)
	{
		UE_LOG(LogTemp, Error, TEXT("UAnimInstancePrimary::SetNewBasePose !NewPose"));
		return;
	}
	
	WeaponBasePoseRef = NewPose;
}