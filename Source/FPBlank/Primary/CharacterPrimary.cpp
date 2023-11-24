#include "CharacterPrimary.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../Base/Weapon_Base.h"
#include <Engine/StreamableManager.h>
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "../Base/HUD_Base.h"
#include "../StaticLibrary.h"
#include "../FreeBattle/PC_FB.h"
#include "GameFramework/GameModeBase.h"
#include "../FreeBattle/GameState_FB.h"
#include "../FreeBattle/PlayerState_FB.h"
#include "AnimInstancePrimary.h"
#include "../FreeBattle/GameMode_FB.h"
#include "../BattleRoyale/GameMode_BR.h"
//#include "Components/ActorComponent.h"
#include "../Base/InventoryComponent.h"


#define PRIMARY 0
#define SECONDARY 1

DEFINE_LOG_CATEGORY_STATIC(CharacterPrimaryLog, All, All);

ACharacterPrimary::ACharacterPrimary()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
	
	CharacterState = 0;
	amountOverlaps = 0;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true; // default - false
	bUseControllerRotationRoll = false;

	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 120.f), FRotator(-10.0f, 90.0f, 0.0f));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Third_PCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third_PCamera"));
	Third_PCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Third_PCamera->bUsePawnControlRotation = false;
	Third_PCamera->bAutoActivate = false;

	First_PCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First_PCamera"));
	First_PCamera->SetupAttachment(GetMesh(), "head");
	First_PCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	First_PCamera->bUsePawnControlRotation = true;
	First_PCamera->bAutoActivate = true;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->SetWalkableFloorAngle(60.0f);
	//FWorldContext* world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
}

void ACharacterPrimary::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UStaticLibrary::CheckRole(this, "SetupPlayerInputComponent");

	DECLARE_DELEGATE_TwoParams(InputDelegate_SB, const FName, const bool);
	DECLARE_DELEGATE_OneParam(InputDelegate_I, int32);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("ChangeView", IE_Pressed, this, &ACharacterPrimary::ChangeView);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterPrimary::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterPrimary::MoveRight);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterPrimary::Run);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterPrimary::StopRun);
	PlayerInputComponent->BindAction("LMB_Action", IE_Pressed, this, &ACharacterPrimary::LMB_Action);
	PlayerInputComponent->BindAction("LMB_Action", IE_Released, this, &ACharacterPrimary::LMB_StopAction);
	PlayerInputComponent->BindAction("RMB_Action", IE_Pressed, this, &ACharacterPrimary::RMB_Action);
	PlayerInputComponent->BindAction("RMB_Action", IE_Released, this, &ACharacterPrimary::RMB_StopAction);
	PlayerInputComponent->BindAction("Reloading", IE_Pressed, this, &ACharacterPrimary::Reloading);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ACharacterPrimary::Interact);
	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &ACharacterPrimary::PauseMenu);
	PlayerInputComponent->BindAction<InputDelegate_I>("PrimaryWeapon", IE_Pressed, this, &ACharacterPrimary::ChangeWeapon, PRIMARY);
	PlayerInputComponent->BindAction<InputDelegate_I>("SecondaryWeapon", IE_Pressed, this, &ACharacterPrimary::ChangeWeapon, SECONDARY);


	if (UStaticLibrary::GameModeIs(AGameMode_BR::StaticClass(), GetWorld()))
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("GameModeIs(AGameMode_BR"));
		PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ACharacterPrimary::ShowInventory);
		PlayerInputComponent->BindAction<InputDelegate_SB>("ChangeBattleMode", IE_Pressed, this, &ACharacterPrimary::ServerCallback, FName("ChangeBattleMode"), true);
	}
	else if (UStaticLibrary::GameModeIs(AGameMode_FB::StaticClass(), GetWorld()))
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("GameModeIs(AGameMode_FB"));
		PlayerInputComponent->BindAction("Score", IE_Pressed, this, &ACharacterPrimary::ShowScore);
		PlayerInputComponent->BindAction("Score", IE_Released, this, &ACharacterPrimary::HideScore);
	}
	else
	{
		UE_LOG(CharacterPrimaryLog, Error, TEXT("Game mode unresolved"));
	}
}

void ACharacterPrimary::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACharacterPrimary::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 60.0f;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -60.0f;

}

void ACharacterPrimary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InteractionLineTrace();
	Inventory->DebugInfo();
}

void ACharacterPrimary::Restart()
{
	Super::Restart();
}

void ACharacterPrimary::MoveForward(float axis)
{
	if (Controller != 0)
	{
		if (axis < 0.f && State(ECharacterState::Runing))
		{
			StopRun();
		}
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, axis);
	}
}

void ACharacterPrimary::MoveRight(float axis)
{
	if (Controller != 0)
	{
		if (!State(ECharacterState::Runing))
		{
			const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
			const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
			AddMovementInput(Direction, axis);
		}
	}
}

void ACharacterPrimary::ChangeView()
{
	if (Third_PCamera->IsActive())
	{
		Third_PCamera->Deactivate();
		First_PCamera->Activate();
	}
	else
	{
		First_PCamera->Deactivate();
		Third_PCamera->Activate();
	}
}

void ACharacterPrimary::ChangeBattleMode()
{
	if (!State(ECharacterState::Firing) && !State(ECharacterState::Aiming) && !State(ECharacterState::PlayingAnim))
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("----------ChangeBattleMode()----------"));
		State(ECharacterState::BattleMode) ? RemoveState(ECharacterState::BattleMode) : AddState(ECharacterState::BattleMode);
		AddState(ECharacterState::PlayingAnim);
		UStaticLibrary::PlayMontage(GetMesh(), AnimChangeBattleMode, this, "ChangeBattleModeNotifyBegin", "AnimMontageEnded");
		
	}
}

void ACharacterPrimary::Run()
{
	if (CanRun())
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("RUN"));
		if (!HasAuthority())
			SetCharacterMovementSpeedRPC(true);
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		AddState(ECharacterState::Runing);
	}
}

void ACharacterPrimary::StopRun()
{
	if (State(ECharacterState::Runing))
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("StopRUN"));
		if (!HasAuthority())
			SetCharacterMovementSpeedRPC(false);
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		RemoveState(ECharacterState::Runing);
	}
}

void ACharacterPrimary::LMB_Action()
{
	if (!GetWorldTimerManager().IsTimerActive(Timer) && CanFire())
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("Can RMB_Action"));
		AddState(ECharacterState::Firing);
		CurrentWeapon->ExecuteAction(EWeaponAction::PrimaryAction);
	}
	else
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("Can`t fire"));
	}
}

void ACharacterPrimary::LMB_StopAction()
{
	if (State(ECharacterState::Firing))
		RemoveState(ECharacterState::Firing);
}

void ACharacterPrimary::RMB_Action()
{
	if (State(ECharacterState::BattleMode) && !State(ECharacterState::PlayingAnim) && !State(ECharacterState::Runing))
	{
		CurrentWeapon->ExecuteAction(EWeaponAction::SecondaryAction);
	}
}

void ACharacterPrimary::RMB_StopAction()
{
	StopAiming();
}

void ACharacterPrimary::Aiming()
{
	if (State(ECharacterState::BattleMode) && !State(ECharacterState::PlayingAnim) && !State(ECharacterState::Runing) /*&& CurrentWeapon->CanAiming()*/)
	{
		AddState(ECharacterState::Aiming);
		CurrentHUD->SetCrosshairVisibility(false);
		static FAttachmentTransformRules attachCameraRules(EAttachmentRule::SnapToTarget, true);
		First_PCamera->bUsePawnControlRotation = false;


		if (First_PCamera->AttachToComponent(CurrentWeapon->GetSkeletalMeshComponent(), attachCameraRules, TEXT("Aiming")))
		{
			UE_LOG(CharacterPrimaryLog, Warning, TEXT("Attach true"));
		}
		First_PCamera->FieldOfView = 75.0f;
	}
}

void ACharacterPrimary::StopAiming()
{
	if (State(ECharacterState::Aiming))
	{
		RemoveState(ECharacterState::Aiming);
		CurrentHUD->SetCrosshairVisibility(true);
		static FAttachmentTransformRules attachCameraRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		First_PCamera->bUsePawnControlRotation = true;
		First_PCamera->AttachToComponent(GetMesh(), attachCameraRules, TEXT("head"));
		First_PCamera->FieldOfView = 90.0f;
	}
}

void ACharacterPrimary::SetWeaponArrayInInventory(TArray<AWeapon_Base*>& _WeaponArray)
{
	if (Inventory)
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("SetWeaponArrayInInventory"));
		Inventory->SetWeaponArray(_WeaponArray);
	}
	
}

void ACharacterPrimary::IOnAddWeapon(AWeapon_Base* NewWeapon)
{
	FName Destination;
	if (!CurrentWeapon.IsValid())
	{
		CurrentWeapon = NewWeapon;
		Destination = ArmedSocket;		
	}
	else
	{
		Destination = GetFreeUnarmedSocket();
	}
	CheckHUD();
	CurrentHUD->AddItemToWidgetInventory(NewWeapon->GetIcon());
	OnAddWeaponServer(NewWeapon, Destination);
}

void ACharacterPrimary::OnAddWeaponServer_Implementation(AWeapon_Base* NewWeapon, FName Destination)
{
	NewWeapon->OnRequestAttachment(this, Destination);
	AttachWeapon(NewWeapon, Destination);
	//NewWeapon->AttachmentRequest(Destination);
}

void ACharacterPrimary::IOnRemoveWeapon(AWeapon_Base* WeaponToRemove)
{
	if (CurrentWeapon == WeaponToRemove)
	{
		CurrentWeapon.Reset();
		RemoveState(ECharacterState::BattleMode);
		CurrentHUD->RemoveWidgetFromViewport(EWidgetList::WeaponInfoWidget);
		CurrentHUD->SetCrosshairVisibility(false);
	}
}

UInventoryWidget* ACharacterPrimary::IGetIventoryWidget()
{
	if (IsLocallyControlled())
		return CurrentHUD->GetInventoryWidget();
	return nullptr;
}
void ACharacterPrimary::ITakeDamage(float damage, FVector InLocation, FVector InNormal)
{
	UE_LOG(CharacterPrimaryLog, Warning, TEXT("ACharacter::ITakeDamage"));
	Super::ITakeDamage(damage, InLocation, InNormal);
	TakeDamageClient(Health);
}

void ACharacterPrimary::DestroyCharacter()
{
	StopAllAction();
	Super::DestroyCharacter();
	APC_FB* PC = Cast<APC_FB>(Controller);
	if (PC)
		GetWorldTimerManager().SetTimer(Timer, PC, &APC_FB::RespawnPlayer, 5.f);
}

void ACharacterPrimary::StopAllAction_Implementation()
{
	LMB_StopAction();
	RMB_StopAction();
}

void ACharacterPrimary::Reloading()
{
	if (CanReloading())
	{
		CurrentWeapon->ExecuteAction(EWeaponAction::Reloading);
	}
}

void ACharacterPrimary::ChangeWeapon(int32 index)
{
	if (CanChangeWeapon())
	{
		if (Inventory->CanChangeWeapon(CurrentWeapon.Get(), index))
		{
			UE_LOG(CharacterPrimaryLog, Display, TEXT("ACharacterPrimary::ChangeWeapon CanChange"));
			AddState(ECharacterState::PlayingAnim);
			AWeapon_Base* NewWeapon = Inventory->TryGetWeaponByIndex(index);
			HasAuthority() ? ChangeWeaponMulticast(CurrentWeapon.Get(), NewWeapon) : ChangeWeaponServer(NewWeapon);
		}
	}
}

void ACharacterPrimary::ChangeWeaponServer_Implementation(AWeapon_Base* NewWeapon)
{
	if (CheckCurrentWeapon())
		ChangeWeaponMulticast(CurrentWeapon.Get(), NewWeapon);
}

void ACharacterPrimary::ChangeWeaponMulticast_Implementation(AWeapon_Base* OldWeapon, AWeapon_Base* NewWeapon)
{
	if (OldWeapon)
		OldWeapon->StopAnimations();
	CurrentWeapon = NewWeapon;
	UStaticLibrary::PlayMontage(GetMesh(), AnimChangeBattleMode, this, "ChangeWeaponNotify", "AnimMontageEnded");
}

void ACharacterPrimary::ShowInventory()
{
	if (CurrentHUD->WidgetInViewport(EWidgetList::InventoryWidget) == false)
	{
		CurrentHUD->AddWidgetToViewport(EWidgetList::InventoryWidget);
	}
	else
	{
		CurrentHUD->RemoveWidgetFromViewport(EWidgetList::InventoryWidget);
	}
}

void ACharacterPrimary::ShowScore()
{
	AGameStateBase* GS = GetWorld()->GetGameState();
	if (GS)
	{
		CurrentHUD->CreateScoreList(GS->PlayerArray);
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("ShowScore"));
	}
}

void ACharacterPrimary::HideScore()
{
	CurrentHUD->RemoveWidgetFromViewport(EWidgetList::ScoreWidget);
}

void ACharacterPrimary::PauseMenu()
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		if (CurrentHUD->WidgetInViewport(EWidgetList::PauseMenuWidget))
		{
			CurrentHUD->RemoveWidgetFromViewport(EWidgetList::PauseMenuWidget);
			PC->SetInputMode(FInputModeGameOnly());
			PC->SetShowMouseCursor(false);
		}
		else
		{
			CurrentHUD->AddWidgetToViewport(EWidgetList::PauseMenuWidget);
			PC->SetInputMode(FInputModeUIOnly());
			PC->SetShowMouseCursor(true);
		}
	}
}

void ACharacterPrimary::SetCharacterMovementSpeedRPC_Implementation(bool shouldRunning)
{
	shouldRunning ? GetCharacterMovement()->MaxWalkSpeed = RunSpeed : GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACharacterPrimary::AddKill()
{
	APlayerState_FB* PS = GetPlayerState<APlayerState_FB>();
	if (PS)
	{
		PS->AddKill();
	}
}

void ACharacterPrimary::IAddDeath()
{
	APlayerState_FB* PS = GetPlayerState<APlayerState_FB>();
	if (PS)
	{
		PS->AddDeath();
	}
}

void ACharacterPrimary::AttachWeapon(AWeapon_Base* WeaponToAttach, FName Destination)
{
	UStaticLibrary::CheckRole(this, "ACharacterPrimary::AttachWeapon");

	if (!Destination.IsNone() && WeaponToAttach)
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("Destination name: %s"), *Destination.ToString());
		FAttachmentTransformRules attachRifleRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		WeaponToAttach->AttachToComponent(GetMesh(), attachRifleRules, Destination);
		if (Destination == ArmedSocket)
		{
			SetBattleIdleBasePose(WeaponToAttach->GetAnimPawnIdle());
		}
		WeaponToAttach->SetWeaponTransformByDestination(Destination);
		if (IsLocallyControlled())
			OnAttachWeaponLocal(Destination == ArmedSocket, WeaponToAttach->GetCurrentAmmo());
	}
	else
	{
		UE_LOG(CharacterPrimaryLog, Error, TEXT("!Destination.IsNone() && WeaponToAttach false"));
	}
}

void ACharacterPrimary::OnAttachWeaponLocal(bool destinationIsArmedSocket, int32 weaponAmmo)
{
	UE_LOG(CharacterPrimaryLog, Warning, TEXT("OnAttachWeaponLocal"));
	if (destinationIsArmedSocket) // weapon was dettached from arm, but attached
	{
		if (!State(ECharacterState::BattleMode))
		{
			AddState(ECharacterState::BattleMode);
		}

		CurrentHUD->SetCrosshairVisibility(true);
		CurrentHUD->AddWidgetToViewport(EWidgetList::WeaponInfoWidget);
		CurrentHUD->SetAmmoInMagazine(weaponAmmo);
	}
	else if (!destinationIsArmedSocket && !ICurrentWeaponInArm()) // weapon was attached, but dettached (x)
	{
		if (State(ECharacterState::BattleMode))
		{
			RemoveState(ECharacterState::BattleMode);
		}
		CurrentHUD->SetCrosshairVisibility(false);
	}
}

bool ACharacterPrimary::CheckHUD()
{
	if (!CurrentHUD)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			CurrentHUD = Cast<AHUD_Base>(PC->MyHUD);
			if (!CurrentHUD)
			{
				UE_LOG(CharacterPrimaryLog, Fatal, TEXT("!CurrentHUD CheckHUD"));
				return false;
			}
		}
	}
	if (!CurrentHUD->WidgetsAlreadyExist())
	{
		CurrentHUD->CreateWidgets();
	}
	return CurrentHUD->WidgetsAlreadyExist();
}

void ACharacterPrimary::Interact()
{
	UE_LOG(CharacterPrimaryLog, Warning, TEXT("Interact Event"));
	if (InteractableActor)
	{
		Inventory->AddItem(InteractableActor);
	}
}

void ACharacterPrimary::AnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(CharacterPrimaryLog, Display, TEXT("AnimMontageEnded"));
	RemoveState(ECharacterState::PlayingAnim);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.Clear();
	GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
}

void ACharacterPrimary::ChangeBattleModeNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UE_LOG(CharacterPrimaryLog, Display, TEXT("ChangeBattleModeNotifyBegin"));
	if (HasAuthority())
	{
		CheckCurrentWeapon();
		if (State(ECharacterState::BattleMode))
		{
			UE_LOG(CharacterPrimaryLog, Error, TEXT("ECharacterState::BattleMode"));
			AttachWeapon(CurrentWeapon.Get(), ArmedSocket);
		}
		else
		{
			UE_LOG(CharacterPrimaryLog, Error, TEXT("!ECharacterState::BattleMode"));
			AttachWeapon(CurrentWeapon.Get(), GetFreeUnarmedSocket());
		}
	}
}

void ACharacterPrimary::ChangeWeaponNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UE_LOG(CharacterPrimaryLog, Display, TEXT("ChangeWeaponNotify"));
	if (!Inventory) 
	{
		UE_LOG(CharacterPrimaryLog, Fatal, TEXT("Inventory nullptr ChangeWeaponNotify"));
	}	
	else
	{
		UE_LOG(CharacterPrimaryLog, Display, TEXT("Inventory ChangeWeaponNotify"));
	}

	if (HasAuthority())
	{
		AWeapon_Base* OldCurrentWeapon = Inventory->GetWeaponAttachedTo(ArmedSocket);
		if (OldCurrentWeapon)
		{
			UE_LOG(CharacterPrimaryLog, Display, TEXT("ACharacterPrimary::ChangeWeaponNotify OldCurrentWeapon"));
			AttachWeapon(OldCurrentWeapon, GetFreeUnarmedSocket());
		}
		else
		{
			UE_LOG(CharacterPrimaryLog, Display, TEXT("ACharacterPrimary::ChangeWeaponNotify !OldCurrentWeapon"));
		}
		CheckCurrentWeapon();
		AttachWeapon(CurrentWeapon.Get(), ArmedSocket);
	}
}

void ACharacterPrimary::InteractionLineTrace()
{
	FVector location;
	FRotator rotation;
	FHitResult hit;
	int32 distance = 500;

	GetController()->GetPlayerViewPoint(location, rotation);

	const FVector start(location);
	const FVector end = start + (rotation.Vector() * distance);

	FCollisionQueryParams traceParams;
	traceParams.MobilityType = EQueryMobilityType::Dynamic;
	traceParams.AddIgnoredActor(this);


	GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, traceParams);

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f);

	if (hit.GetActor())
	{
		UE_LOG(CharacterPrimaryLog, Warning, TEXT("NameComponent: %s"), *hit.Component.Get()->GetFName().ToString());
	}

	if (hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), hit.ImpactPoint, FVector(5, 5, 5), FColor::Blue, false, 2.0f);
	}

	InteractableActor = hit.GetActor();
	InteractableActor ? CurrentHUD->SetInteractionWidgetState(InteractableActor->IGetItemName()) : CurrentHUD->SetInteractionWidgetState(NAME_None);
}

void ACharacterPrimary::BeginOverlapWithInteractable()
{
	amountOverlaps++;
	SetActorTickEnabled(true);
}

void ACharacterPrimary::EndOverlapWithInteractable()
{
	amountOverlaps--;
	InteractableActor = nullptr;
	CurrentHUD->SetInteractionWidgetState(NAME_None);
	if (amountOverlaps == 0)
	{
		SetActorTickEnabled(false);
	}
}

FName ACharacterPrimary::GetFreeUnarmedSocket()
{
	TArray<USceneComponent*> ChildrenComponents;
	TArray<FName> ChildrenSockets;
	GetMesh()->GetChildrenComponents(false, ChildrenComponents);

	for (USceneComponent*& CurrentChild : ChildrenComponents)
	{
		ChildrenSockets.Add(CurrentChild->GetAttachSocketName());
	}
	for (const FName& CurrentSocket : UnarmedSockets)
	{
		if (ChildrenSockets.Find(CurrentSocket) == INDEX_NONE)
			return CurrentSocket;
	}
	return NAME_None;
}

bool ACharacterPrimary::ICurrentWeaponInArm()
{
	if (CurrentWeapon.IsValid())
		return CurrentWeapon->GetAttachParentSocketName() == ArmedSocket;
	return false;
}

bool ACharacterPrimary::CheckCurrentWeapon()
{
	AWeapon_Base* CheckCurrentInInveotry = Inventory->GetWeaponAttachedTo(ArmedSocket);
	if (CheckCurrentInInveotry)
	{
		CurrentWeapon = CheckCurrentInInveotry;
	}
	return CurrentWeapon.IsValid();
}

bool ACharacterPrimary::CanFire() const
{
	return State(ECharacterState::BattleMode) && !State(ECharacterState::PlayingAnim) && !State(ECharacterState::Runing)
		&& CurrentWeapon.Get();
}

bool ACharacterPrimary::CanReloading() const
{
	return State(ECharacterState::BattleMode) && !State(ECharacterState::Aiming) && !State(ECharacterState::Firing)
		&& !State(ECharacterState::PlayingAnim) && !State(ECharacterState::Runing)
		&& !CurrentWeapon.Get()->MagazineIsFull();
}

bool ACharacterPrimary::CanRun() const
{
	return !State(ECharacterState::Aiming) && !State(ECharacterState::Firing) && !State(ECharacterState::PlayingAnim) 
		&& (GetInputAxisValue("MoveForward") == 1.f) && (GetInputAxisValue("MoveRight") == 0.f);
}

bool ACharacterPrimary::CanChangeWeapon() const
{
	return !State(ECharacterState::Firing) && !State(ECharacterState::Aiming) && !State(ECharacterState::PlayingAnim) 
		&& !State(ECharacterState::Runing);
}

ACharacterPrimary::~ACharacterPrimary()
{

}