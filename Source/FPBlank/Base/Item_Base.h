#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Base.generated.h"



class UBoxComponent;

UCLASS()
class FPBLANK_API AItem_Base : public AActor
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly);
	UBoxComponent* BoxCollisionOutside;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly);
	UBoxComponent* BoxCollisionInside;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	FName ItemName;
	
public:	
	AItem_Base();
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }

	UFUNCTION()
	virtual void SetCollisionState(bool enabled);

protected:
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
