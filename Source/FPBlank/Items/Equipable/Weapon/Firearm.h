#pragma once
#include "CoreMinimal.h"
#include "../../../Base/Weapon_Base.h"
#include "Components/TimelineComponent.h"
#include "LegacyCameraShake.h"
#include "Firearm.generated.h"




UENUM(BlueprintType)
enum class EFireMode : uint8
{
	None,
	Auto,
	Single,
	Birst
};

UENUM(BlueprintType, Meta = (Bitflags))
enum class EPossibleFireMode : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Auto = 1 UMETA(DisplayName = "Auto"),
	Single = 2 UMETA(DisplayName = "Single"),
	Birst = 4 UMETA(DisplayName = "Birst")
};


class UTimelineComponent;
class AShell;
class AMagazine;
class UParticleSystem;
class UMaterialInterface;
class UProjectileComponent;



UCLASS(Blueprintable, BlueprintType)
class FPBLANK_API AFirearm : public AWeapon_Base
{
	GENERATED_BODY()

protected:
	UPROPERTY();
	UTimelineComponent* TimelineRecoilSystem;

	UPROPERTY(VisibleDefaultsOnly);
	UProjectileComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Recoil");
	UCurveFloat* RecoilCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation");
	UAnimMontage* WeaponAnimFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation");
	UAnimMontage* PawnAnimFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Recoil");
	TSubclassOf<ULegacyCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Effects");
	USoundBase* DryFireSound;

	UPROPERTY(BlueprintReadWrite);
	AMagazine* CurrentMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Components");
	TSubclassOf<AMagazine> MagazineEmptyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Components");
	TSubclassOf<AMagazine> MagazineFullClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Components");
	TSubclassOf<AShell> ShellClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Sockets");
	FTransform SocketMagazineTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Sockets");
	FTransform SocketMuzzle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Characteristics");
	int32 fireRate;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Characteristics");
	float fireRateDelay;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Characteristics");
	float headMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "EPossibleFireMode"), Category = "Weapon|Characteristics");
	uint8 possibleFireMode;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Characteristics");
	EFireMode fireMode;

public:
	AFirearm();
	virtual void Tick(float DeltaTime) override;
	bool CanAiming();
	void ExecuteAction(EWeaponAction Action) override;

	FORCEINLINE float GetHeadMultiplier() const { return headMultiplier; };
	FORCEINLINE float GetFireRateDelay() const { return fireRateDelay; };
	

	UFUNCTION()
		void RecoilInterpProgress(float value);

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void FireAI();

	UFUNCTION(BlueprintImplementableEvent)
		void Reloading(int32 _currentAmmo);

	UFUNCTION(Server, Reliable)
		void ReloadingServer(int32 _currentAmmo);

	UFUNCTION(NetMulticast, Reliable)
		void ReloadingMulticast(int32 _currentAmmo);

	UFUNCTION()
		void FireReplication();
	

	UFUNCTION(Server, Reliable)
		void ServerCallback(FName FunctionCallbackName, bool Multicast = false);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastCallback(FName FunctionCallbackName);

	UFUNCTION(Server, Unreliable)
		void UnreliableServerCallback(FName FunctionCallbackName, bool Multicast = false);

	UFUNCTION(NetMulticast, Unreliable)
		void UnreliableMulticastCallback(FName FunctionCallbackName);

protected:
	void BeginPlay() override;

private:
	void ResetTimeline();
};
