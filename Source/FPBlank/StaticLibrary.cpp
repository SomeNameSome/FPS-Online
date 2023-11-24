#include "StaticLibrary.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

DEFINE_LOG_CATEGORY_STATIC(StaticLibraryLog, All, All);

float UStaticLibrary::PlayMontage(USkeletalMeshComponent* SkeletalMesh, UAnimMontage* MontageToPlay, AActor* NotifyFuncOwner, 
	FName NotifyBeginFunc, FName OnMontageEnded, float playRate, float startingPosition, FName startingSection)
{
	float montageLength = 0.f;
	if (SkeletalMesh->IsValidLowLevelFast())
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if (AnimInstance->IsValidLowLevelFast())
		{
			if (MontageToPlay->IsValidLowLevelFast())
			{
				montageLength = AnimInstance->Montage_Play(MontageToPlay, playRate, EMontagePlayReturnType::MontageLength, startingPosition);
				if (montageLength > 0.f)
				{
					if (!AnimInstance->OnPlayMontageNotifyBegin.IsBound())
					{
						UE_LOG(StaticLibraryLog, Warning, TEXT("IsBound() == false"));

						if (NotifyBeginFunc != NAME_None && NotifyFuncOwner)
						{
							UE_LOG(StaticLibraryLog, Display, TEXT("NotifyBeginFunc != NAME_None"));
							FScriptDelegate BeginDelegate;
							BeginDelegate.BindUFunction(NotifyFuncOwner, NotifyBeginFunc);
							AnimInstance->OnPlayMontageNotifyBegin.Add(BeginDelegate);
						}
						if (OnMontageEnded != NAME_None && NotifyFuncOwner)
						{
							UE_LOG(StaticLibraryLog, Display, TEXT("NotifyEndFunc != NAME_None"));
							FScriptDelegate EndDelegate;
							EndDelegate.BindUFunction(NotifyFuncOwner, OnMontageEnded);
							AnimInstance->OnMontageEnded.Add(EndDelegate);
						}
					}
					else
					{
						UE_LOG(StaticLibraryLog, Warning, TEXT("IsBound() already"));
					}
					return montageLength;
				}
				else
				{
					UE_LOG(StaticLibraryLog, Warning, TEXT("montageLength == 0.f"));
					if (MontageToPlay->IsValidLowLevelFast())
					{
						UE_LOG(StaticLibraryLog, Warning, TEXT("Is ValidToPlay"));
					}
					else
					{
						UE_LOG(StaticLibraryLog, Warning, TEXT("Is ValidToPlay == false"));
					}
				}
			}
			else
			{
				UE_LOG(StaticLibraryLog, Warning, TEXT("MontageToPlay is invalid"));
			}
		}
		else
		{
			UE_LOG(StaticLibraryLog, Warning, TEXT("AnimInstance not valid"));
		}
	}
	else
	{
		UE_LOG(StaticLibraryLog, Warning, TEXT("GetMesh() is corrupted"));
	}
	return montageLength;
}


void UStaticLibrary::GetPlayerViewPoint(AController* Controller, FVector& InVector, FRotator& InRotator)
{
	if (Controller)
	{
		Controller->GetPlayerViewPoint(InVector, InRotator);
	}
}

void UStaticLibrary::CheckRole(AActor* ActorToCheck, FString FromFunc)
{
	UE_LOG(StaticLibraryLog, Error, TEXT("----CheckRole---- %s"), *FromFunc);
	if (ActorToCheck)
	{
		if (ActorToCheck->HasAuthority())
		{
			UE_LOG(StaticLibraryLog, Warning, TEXT("HasAuthority"));
		}
		else
		{
			UE_LOG(StaticLibraryLog, Warning, TEXT("!HasAuthority"));
		}
		if (APawn* PawnToCheck = Cast<APawn>(ActorToCheck))
		{
			if (PawnToCheck->IsLocallyControlled())
			{
				UE_LOG(StaticLibraryLog, Warning, TEXT("IsLocallyControlled"));
			}
			else
			{
				UE_LOG(StaticLibraryLog, Warning, TEXT("!IsLocallyControlled"));
			}
		}
	}
	else
	{
		UE_LOG(StaticLibraryLog, Error, TEXT("!ActorToCheck"));
	}
}

bool UStaticLibrary::GameModeIs(UClass* GameMode, UWorld* WorldContext)
{
	UClass* GM_Class = UGameplayStatics::GetGameState(WorldContext)->GameModeClass;
	return GM_Class->IsChildOf(GameMode) || GM_Class == GameMode;
}

void UStaticLibrary::ClearPrintDebugLog()
{
	GEngine->ClearOnScreenDebugMessages();
}