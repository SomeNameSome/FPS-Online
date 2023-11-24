#include "GameInstancePrimary.h"



void UGameInstancePrimary::SetEnableAI(bool enable)
{
	enableAI = enable;
}

bool UGameInstancePrimary::AIIsEnabled()
{
	return enableAI;
}