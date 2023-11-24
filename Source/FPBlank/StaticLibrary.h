#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StaticLibrary.generated.h"



UCLASS()
class FPBLANK_API UStaticLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static float PlayMontage(USkeletalMeshComponent* SkeletalMesh, UAnimMontage* MontageToPlay, AActor* NotifyFuncOwner = nullptr, 
		FName NotifyBeginFunc = NAME_None, FName OnMontageEnded = NAME_None, float playRate = 1.0f, float startingPosition = 0.0f,
		FName startingSection = NAME_None);

	UFUNCTION(BlueprintPure)
	static void GetPlayerViewPoint(AController* Controller, FVector& InVector, FRotator& InRotator);

	static void CheckRole(AActor* ActorToCheck, FString FromFunc = "None");

	static bool GameModeIs(UClass* GameMode, UWorld* WorldContext);

	UFUNCTION(BlueprintCallable)
	static void ClearPrintDebugLog();
};
