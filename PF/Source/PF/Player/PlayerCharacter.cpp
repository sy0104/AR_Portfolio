// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "../PFGameInstance.h"
#include "../Manager/InventoryManager.h"
#include "../PFGameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UMG/InventoryBase.h"
#include "../UMG/InventoryItemBase.h"
#include "../UMG/ItemDataBase.h"
#include "../PFSaveGame.h"
#include "../PFGameModeBase.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	mSpringArm->SetupAttachment(GetMesh());
	mCamera->SetupAttachment(mSpringArm);

	mSpringArm->TargetArmLength = 500.f;
	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	mSpringArm->SetRelativeRotation(FRotator(-15.0, 90.0, 0.0));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetCharacterMovement()->JumpZVelocity = 600.f;

	mMoveDir = 0.f;
	mDeath = false;
	mUseSkill = false;
	mTargeting = false;
	mDash = false;

	mCameraZoomMin = 100.f;
	mCameraZoomMax = 500.f;

	mCameraZoom = false;
	mCameraZoomSpeed = 10.f;
	mCameraZoomIn = false;
	mSkillCameraZoomMin = 200.f;
	mSkillCameraZoomMax = 500.f;

	mHPRatio = 1.f;
	mMPRatio = 1.f;
	mExpRatio = 0.f;

	mCurMPTime = 0.f;
	mMPRecoveryTime = 3.f;

	mAttackBuff.bBuffOn = false;
	mAttackBuff.CurBuffTime = 0.f;
	mAttackBuff.BuffTime = 10.f;

	mArmorBuff.bBuffOn = false;
	mArmorBuff.CurBuffTime = 0.f;
	mArmorBuff.BuffTime = 10.f;

	// Camera Shake BPC
	// Damage
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> DamageCameraShakeBPC(
		TEXT("Blueprint'/Game/Blueprints/Camera/BPC_DamageCameraShake.BPC_DamageCameraShake_C'"));

	if (DamageCameraShakeBPC.Succeeded())
		mDamageCameraShake = DamageCameraShakeBPC.Class;

	// Skill
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> SkillCameraShakeBPC(
		TEXT("Blueprint'/Game/Blueprints/Camera/BPC_SkillCameraShake.BPC_SkillCameraShake_C'"));

	if (SkillCameraShakeBPC.Succeeded())
		mSkillCameraShake = SkillCameraShakeBPC.Class;

	// Ultimate
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> UltimateCameraShakeBPC(
		TEXT("Blueprint'/Game/Blueprints/Camera/BPC_UltimateCameraShake.BPC_UltimateCameraShake_C'"));

	if (UltimateCameraShakeBPC.Succeeded())
		mUltimateCameraShake = UltimateCameraShakeBPC.Class;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	UPFGameInstance* GameInst = GetWorld()->GetGameInstance<UPFGameInstance>();

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	//UInventoryManager::GetInst(GetWorld())->ShowInventory(false);

	// Read
	FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");
	TSharedPtr<FArchive> Reader = MakeShareable(IFileManager::Get().CreateFileReader(*FullPath));

	int32 WeaponIndex = -1, ArmorIndex = -1, AccesaryIndex = -1;

	if (Reader.IsValid())
	{
		// Player Info
		*Reader.Get() << mPlayerInfo.Name;
		*Reader.Get() << mPlayerInfo.AttackPoint;
		*Reader.Get() << mPlayerInfo.ArmorPoint;
		*Reader.Get() << mPlayerInfo.HP;
		*Reader.Get() << mPlayerInfo.HPMax;
		*Reader.Get() << mPlayerInfo.MP;
		*Reader.Get() << mPlayerInfo.MPMax;
		*Reader.Get() << mPlayerInfo.Level;
		*Reader.Get() << mPlayerInfo.Exp;
		*Reader.Get() << mPlayerInfo.Gold;
		*Reader.Get() << mPlayerInfo.MoveSpeed;
		*Reader.Get() << mPlayerInfo.AttackDistance;

		*Reader.Get() << mCameraZoomMin;
		*Reader.Get() << mCameraZoomMax;

		*Reader.Get() << mHPRatio;
		*Reader.Get() << mMPRatio;
		*Reader.Get() << mExpRatio;

		// Potion Count
		int32 HPPotionCount, MPPotionCount, AttackPotionCount, ArmorPotionCount;

		*Reader.Get() << HPPotionCount;
		*Reader.Get() << MPPotionCount;
		*Reader.Get() << AttackPotionCount;
		*Reader.Get() << ArmorPotionCount;

		MainHUD->SetHPPotionCount(HPPotionCount);
		MainHUD->SetMPPotionCount(MPPotionCount);
		MainHUD->SetAttackPotionCount(AttackPotionCount);
		MainHUD->SetArmorPotionCount(ArmorPotionCount);

		// Equiped Item Index
		*Reader.Get() << mEquipedWeaponIndex;
		*Reader.Get() << mEquipedArmorIndex;
		*Reader.Get() << mEquipedAccesaryIndex;
	}


	// 나중에 직업 추가되면 데이터테이블로 제작
	else
	{
		mPlayerInfo.Name = TEXT("Shinbi");
		mPlayerInfo.AttackPoint = 200;	// 100
		mPlayerInfo.ArmorPoint = 50;
		mPlayerInfo.HP = 1000;
		mPlayerInfo.HPMax = 1000;
		mPlayerInfo.MP = 100;
		mPlayerInfo.MPMax = 100;
		mPlayerInfo.Level = 1;
		mPlayerInfo.Exp = 0;
		mPlayerInfo.ExpMax = 1000;
		mPlayerInfo.Gold = 1000;
		mPlayerInfo.MoveSpeed = 2000.f;
		mPlayerInfo.AttackDistance = 200.f;

		mEquipedWeaponIndex = -1;
		mEquipedArmorIndex = -1;
		mEquipedAccesaryIndex = -1;
	}

	// Inven Load
	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();

	if (IsValid(TileView))
	{
		TArray<UObject*> ListItems = TileView->GetListItems();

		TArray<FSaveItemInfo> SaveItemArray = GameInst->GetSaveInven();

		int32 Count = SaveItemArray.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			EItemID ItemID = SaveItemArray[i].ID;
			int32 ItemCount = SaveItemArray[i].ItemCount;

			FItemDataInfo* ItemInfo = UInventoryManager::GetInst(GetWorld())->GetItemInfo(ItemID);
			UItemDataBase* ItemData = NewObject<UItemDataBase>();

			ItemData->SetItemInfo(ItemInfo);
			ItemData->SetItemCount(ItemCount);
			TileView->AddItem(ItemData);
		}
	}

	// Equip Item

	if (IsValid(TileView))
	{
		if (mEquipedWeaponIndex != -1)
		{
			UObject* Weapon = TileView->GetItemAt(mEquipedWeaponIndex);

			if (IsValid(Weapon))
			{
				EquipItem(Weapon);
			}
		}

		if (mEquipedArmorIndex != -1)
		{
			UObject* Armor = TileView->GetItemAt(mEquipedArmorIndex);

			if (IsValid(Armor))
			{
				EquipItem(Armor);
			}
		}

		if (mEquipedAccesaryIndex != -1)
		{
			UObject* Accesary = TileView->GetItemAt(mEquipedAccesaryIndex);

			if (IsValid(Accesary))
			{
				EquipItem(Accesary);
			}
		}
	}

	// Player Skill
	int32 SkillCount = mSkillNameArray.Num();

	for (int32 i = 0; i < SkillCount; ++i)
	{
		const FSkillData* Data = GameInst->FindPlayerSkillTable(mSkillNameArray[i]);

		FPlayerSkillInfo SkillInfo;

		SkillInfo.Type = Data->Type;
		SkillInfo.System = Data->System;
		SkillInfo.SkillName = Data->SkillName;
		SkillInfo.SkillOptionArray = Data->SkillOptionArray;
		SkillInfo.Distance = Data->Distance;
		SkillInfo.MP = Data->MP;

		mSkillDataArray.Add(SkillInfo);
	}

	InitUI();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SavePlayer();

	PrintViewport(1.f, FColor::Red, TEXT("Player::EndPlay"));
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("AttackPoint : %d"), mPlayerInfo.AttackPoint));

	AttackBuffOn(DeltaTime);
	ArmorBuffOn(DeltaTime);

	CameraZoomInOut();
	RecoveryMP(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 액션 매핑
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::NormalAttack);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillQ"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillQKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillE"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillEKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillR"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillRKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillRM"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillRMKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillF"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillFKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InventoryOn);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Stat"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerStatOnOff);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Cheat"), EInputEvent::IE_Pressed, this, &APlayerCharacter::CheatKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("LevelUpCheat"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LevelUpCheat);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Item"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ItemKey);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("HPPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::HPPotionkey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("MPPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::MPPotionKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("AttackPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::AttackPotionKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("ArmorPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ArmorPotionKey);

	// 축 매핑
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraZ"), this, &APlayerCharacter::RotationCameraZ);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraY"), this, &APlayerCharacter::RotationCameraY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoom);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (mUseSkill)
		return 0.f;

	UGameplayStatics::PlayWorldCameraShake(GetWorld(), mDamageCameraShake, GetActorLocation(), 100.f, 2000.f, 1.f, false);

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage = Damage - mPlayerInfo.ArmorPoint;
	Damage = Damage < 1 ? 1 : Damage;

	mPlayerInfo.HP -= Damage;

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Damage: %f"), Damage));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("HP: %d"), mPlayerInfo.HP));

	if (mPlayerInfo.HP <= 0)	// 죽음
	{
		APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();
		MainHUD->SetHP(0.f);

		mDeath = true;
		mAnimInst->Death();
	}

	else
	{
		mAnimInst->Hit();

		// HP UI
		APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();

		mHPRatio -= Damage * (1.f / (float)mPlayerInfo.HPMax);
		MainHUD->SetHP(mHPRatio);
		MainHUD->SetPlayerStatHP(mPlayerInfo.HP);
	}

	return Damage;
}

void APlayerCharacter::UnPossessed()
{
	Super::UnPossessed();

}

void APlayerCharacter::MoveFront(float Scale)
{
	if (mDeath || mUseSkill || mTargeting || mDash)
		return;

	mMoveDir = Scale;

	if (Scale == 0.f)
		return;

	AddMovementInput(GetActorForwardVector(), Scale);
}

void APlayerCharacter::MoveSide(float Scale)
{
	if (mDeath || mUseSkill || mTargeting || mDash)
		return;

	if (mMoveDir == 1.f)	// W
	{
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);

		else if (Scale == 1.f)	// 오른쪽 이동
			mAnimInst->SetMoveDir(45.f);

		else if (Scale == -1.f)	// 왼쪽 이동
			mAnimInst->SetMoveDir(-45.f);
	}

	else if (mMoveDir == -1.f)	// S
	{
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(180.f);

		else if (Scale == 1.f)	// 오른쪽 이동
			mAnimInst->SetMoveDir(135.f);

		else if (Scale == -1.f)	// 왼쪽 이동
			mAnimInst->SetMoveDir(-135.f);
	}

	else
	{
		if (Scale == 0.f)		// 아무 키도 누른게 없을 경우
			mAnimInst->SetMoveDir(0.f);

		else if (Scale == 1.f)	// 오른쪽 이동
			mAnimInst->SetMoveDir(90.f);

		else if (Scale == -1.f)	// 왼쪽 이동
			mAnimInst->SetMoveDir(-90.f);
	}

	if (Scale == 0.f)
		return;

	AddMovementInput(GetActorRightVector(), Scale);
}

void APlayerCharacter::RotationCameraZ(float Scale)
{
	if (mDeath || mDash)
		return;

	if (Scale == 0.f)
		return;

	AddControllerYawInput(Scale * 180.f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::RotationCameraY(float Scale)
{
	if (mDash)
		return;

	if (Scale == 0.f)
		return;

	mSpringArm->AddRelativeRotation(FRotator(Scale * 180.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
}

void APlayerCharacter::CameraZoom(float Scale)
{
	if (Scale == 0.f || mTargeting || mDash)
		return;

	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Scale : %.5f"), Scale));

	mSpringArm->TargetArmLength += Scale * -5.f;

	if (mSpringArm->TargetArmLength < mCameraZoomMin)
		mSpringArm->TargetArmLength = mCameraZoomMin;

	if (mSpringArm->TargetArmLength > mCameraZoomMax)
		mSpringArm->TargetArmLength = mCameraZoomMax;
}

void APlayerCharacter::NormalAttack()
{
	if (mDeath || mUseSkill || mTargeting || mDash)
		return;

	mAnimInst->Attack();
}

void APlayerCharacter::SkillQKey()
{
	if (mDeath || mTargeting)
		return;

	int32 SkillNum = (int32)EPlayerSkillKey::SkillQ;

	if (mPlayerInfo.MP < mSkillDataArray[SkillNum].MP)
		return;

	//mAnimInst->UseSkill(0);

	// 마나
	mPlayerInfo.MP -= mSkillDataArray[SkillNum].MP;

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	mMPRatio -= mSkillDataArray[SkillNum].MP * (1.f / (float)mPlayerInfo.MPMax);
	MainHUD->SetMP(mMPRatio);
	MainHUD->SetPlayerStatMP(mPlayerInfo.MP);
}

void APlayerCharacter::SkillEKey()
{
	if (mDeath || mTargeting)
		return;

	//mAnimInst->UseSkill(1);

	int32 SkillNum = (int32)EPlayerSkillKey::SkillE;

	if (mPlayerInfo.MP < mSkillDataArray[SkillNum].MP)
		return;

	// 마나
	mPlayerInfo.MP -= mSkillDataArray[SkillNum].MP;

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	mMPRatio -= mSkillDataArray[SkillNum].MP * (1.f / (float)mPlayerInfo.MPMax);
	MainHUD->SetMP(mMPRatio);
	MainHUD->SetPlayerStatMP(mPlayerInfo.MP);

}

void APlayerCharacter::SkillRKey()
{
	if (mDeath)
		return;

	//mAnimInst->UseSkill(2);

	int32 SkillNum = (int32)EPlayerSkillKey::SkillR;

	if (mPlayerInfo.MP < mSkillDataArray[SkillNum].MP)
		return;

	// 마나
	mPlayerInfo.MP -= mSkillDataArray[SkillNum].MP;

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	mMPRatio -= mSkillDataArray[SkillNum].MP * (1.f / (float)mPlayerInfo.MPMax);
	MainHUD->SetMP(mMPRatio);
	MainHUD->SetPlayerStatMP(mPlayerInfo.MP);

}

void APlayerCharacter::SkillFKey()
{
	if (mDeath || mTargeting)
		return;

	PrintViewport(1.f, FColor::Red, TEXT("SkillFKey()"));

	//mAnimInst->UseSkill(3);
}

void APlayerCharacter::SkillRMKey()
{
	if (mDeath || mTargeting)
		return;


	int32 SkillNum = (int32)EPlayerSkillKey::SkillRM;

	if (mPlayerInfo.MP < mSkillDataArray[SkillNum].MP)
		return;

	mAnimInst->UseSkill(3);

	// 마나
	mPlayerInfo.MP -= mSkillDataArray[SkillNum].MP;

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	mMPRatio -= mSkillDataArray[SkillNum].MP * (1.f / (float)mPlayerInfo.MPMax);
	MainHUD->SetMP(mMPRatio);
	MainHUD->SetPlayerStatMP(mPlayerInfo.MP);

}

void APlayerCharacter::JumpKey()
{
	if (mDeath || mUseSkill || mTargeting || mDash)
		return;

	else if (mAnimInst->GetPlayerAnimType() != EPlayerAnimType::Ground)
		return;

	Jump();
	mAnimInst->Jump();
}

void APlayerCharacter::InventoryOn()
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();

	if (IsValid(TileView))
	{
		TArray<UObject*> mTileViewItems = TileView->GetListItems();
		int32 Count = mTileViewItems.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			if (i == mEquipedWeaponIndex || i == mEquipedArmorIndex || i == mEquipedAccesaryIndex)
			{
				UItemDataBase* EquipedItem = Cast<UItemDataBase>(TileView->GetItemAt(i));
				//UObject* EquipedItem =TileView->GetItemAt(i);
				UInventoryItemBase* EquipedItemBase = Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(EquipedItem));

				if (IsValid(EquipedItemBase))
				{
					EquipedItemBase->SetEquip();
				}
			}
		}
	}

	if (UInventoryManager::GetInst(GetWorld())->IsInventoryOpen())
		UInventoryManager::GetInst(GetWorld())->ShowInventory(false);

	else
	{
		// Set Equip

		if (IsValid(TileView))
		{
			TArray<UObject*> mTileViewItems = TileView->GetListItems();
			int32 Count = mTileViewItems.Num();

			for (int32 i = 0; i < Count; ++i)
			{
				if (i == mEquipedWeaponIndex || i == mEquipedArmorIndex || i == mEquipedAccesaryIndex)
				{
					UItemDataBase* EquipedItem = Cast<UItemDataBase>(TileView->GetItemAt(i));
					//UObject* EquipedItem =TileView->GetItemAt(i);
					UInventoryItemBase* EquipedItemBase = Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(EquipedItem));

					if (IsValid(EquipedItemBase))
					{
						EquipedItemBase->SetEquip();
					}
				}
			}
		}

		UInventoryManager::GetInst(GetWorld())->ShowInventory(true);
	}
}

void APlayerCharacter::PlayerStatOnOff()
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	if (MainHUD->IsPlayerStatOpen())
		MainHUD->SetPlayerStatVisible(false);
	else
		MainHUD->SetPlayerStatVisible(true);
}

void APlayerCharacter::CheatKey()
{
	mPlayerInfo.HP = mPlayerInfo.HPMax;
	mPlayerInfo.MP = mPlayerInfo.MPMax;

	mHPRatio = 1.f;
	mMPRatio = 1.f;

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	MainHUD->SetHP(mHPRatio);
	MainHUD->SetMP(mMPRatio);
	MainHUD->SetPlayerStatHP(mPlayerInfo.HP);
	MainHUD->SetPlayerStatMP(mPlayerInfo.MP);
}

void APlayerCharacter::ItemKey()
{

}

void APlayerCharacter::HPPotionkey()
{
	FItemDataInfo* Item = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_HP_Potion);

	if (Item->ItemCount <= 0)
		return;

	UsePotion(Item);
	SetPotionInfo(Item);
}

void APlayerCharacter::MPPotionKey()
{
	FItemDataInfo* Item = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_MP_Potion);
	if (Item->ItemCount <= 0)
		return;

	UsePotion(Item);
	SetPotionInfo(Item);

}

void APlayerCharacter::AttackPotionKey()
{
	FItemDataInfo* Item = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_Attack_Potion);
	if (Item->ItemCount <= 0)
		return;

	UsePotion(Item);
	SetPotionInfo(Item);
}

void APlayerCharacter::ArmorPotionKey()
{
	FItemDataInfo* Item = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_Armor_Potion);
	if (Item->ItemCount <= 0)
		return;

	UsePotion(Item);
	SetPotionInfo(Item);
}

void APlayerCharacter::NormalAttackCheck()
{
}

void APlayerCharacter::UseSkill(int32 SkillNumber)
{
}

void APlayerCharacter::SkillQ()
{
}

void APlayerCharacter::SkillE()
{
}

void APlayerCharacter::SkillR()
{
}

void APlayerCharacter::SkillRM()
{
}

void APlayerCharacter::SkillF()
{
}

void APlayerCharacter::SkillRCast()
{
}

void APlayerCharacter::SkillRMCast()
{
}

void APlayerCharacter::LevelUp()
{
	mPlayerInfo.Level++;
	mPlayerInfo.AttackPoint += 10;
	mPlayerInfo.ArmorPoint += 5;
	mPlayerInfo.HPMax += 100;
	mPlayerInfo.HP = mPlayerInfo.HPMax;
	mPlayerInfo.MPMax += 10;
	mPlayerInfo.MP = mPlayerInfo.MPMax;
	mPlayerInfo.ExpMax += mPlayerInfo.ExpMax / 2;

	mHPRatio = 1.f;
	mMPRatio = 1.f;
	mExpRatio = 0.f;
}

void APlayerCharacter::LevelUpCheat()
{
	LevelUp();

	UpdatePlayerInfoUI();
}

void APlayerCharacter::UpdatePlayerInfoUI()
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	// PlayerInfo UI
	MainHUD->SetHP(mHPRatio);
	MainHUD->SetMP(mMPRatio);
	MainHUD->SetExp(mExpRatio);
	MainHUD->SetLevel(mPlayerInfo.Level);

	// PlayerStat UI
	MainHUD->SetPlayerStatAttackPoint(mPlayerInfo.AttackPoint);
	MainHUD->SetPlayerStatArmorPoint(mPlayerInfo.ArmorPoint);
	MainHUD->SetPlayerStatHP(mPlayerInfo.HP);
	MainHUD->SetPlayerStatMP(mPlayerInfo.MP);
	MainHUD->SetPlayerStatHPMax(mPlayerInfo.HPMax);
	MainHUD->SetPlayerStatMPMax(mPlayerInfo.MPMax);
}

void APlayerCharacter::UsePotion(FItemDataInfo* ItemInfo)
{
	// 버프 아이템 - 포션

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	switch (ItemInfo->ID)
	{
	case EItemID::PT_HP_Potion:
		mPlayerInfo.HP += ItemInfo->HPHeal;

		if (mPlayerInfo.HP >= mPlayerInfo.HPMax)
			mPlayerInfo.HP = mPlayerInfo.HPMax;

		mHPRatio += ItemInfo->HPHeal * (1.f / (float)mPlayerInfo.HPMax);
		mHPRatio = mHPRatio >= 1.f ? 1.f : mHPRatio;

		MainHUD->SetHP(mHPRatio);
		MainHUD->SetPlayerStatHP(mPlayerInfo.HP);
		break;

	case EItemID::PT_MP_Potion:
		mPlayerInfo.MP += ItemInfo->MPHeal;

		if (mPlayerInfo.MP >= mPlayerInfo.MPMax)
			mPlayerInfo.MP = mPlayerInfo.MPMax;

		mMPRatio += ItemInfo->MPHeal * (1.f / (float)mPlayerInfo.MPMax);
		mMPRatio = mMPRatio >= 1.f ? 1.f : mMPRatio;

		MainHUD->SetMP(mMPRatio);
		MainHUD->SetPlayerStatMP(mPlayerInfo.MP);
		break;

	case EItemID::PT_Attack_Potion:
		mAttackBuff.bBuffOn = true;
		mAttackBuff.BuffValue = ItemInfo->AttackBuff;
		mAttackBuff.OriginStat = mPlayerInfo.AttackPoint;
		MainHUD->SetAttackBuffText((int32)ItemInfo->AttackBuff);
		break;

	case EItemID::PT_Armor_Potion:
		mArmorBuff.bBuffOn = true;
		mArmorBuff.BuffValue = ItemInfo->ArmorBuff;
		mArmorBuff.OriginStat = mPlayerInfo.ArmorPoint;
		MainHUD->SetArmorBuffText((int32)ItemInfo->ArmorBuff);
		break;
	}


}

void APlayerCharacter::InitUI()
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	if (IsValid(MainHUD))
	{
		// Player Info UI
		MainHUD->SetHP(mHPRatio);
		MainHUD->SetMP(mMPRatio);
		MainHUD->SetExp(mExpRatio);
		MainHUD->SetLevel(mPlayerInfo.Level);
		MainHUD->SetGold(mPlayerInfo.Gold);

		// Player Stat UI
		MainHUD->SetPlayerStatUI(mPlayerInfo);
		MainHUD->SetPlayerStatVisible(false);

		//// Potion Info UI
		//MainHUD->SetHPPotionCount(0);
		//MainHUD->SetMPPotionCount(0);
		//MainHUD->SetAttackPotionCount(0);
		//MainHUD->SetArmorPotionCount(0);
	}
}

void APlayerCharacter::SetPotionInfo(FItemDataInfo* ItemInfo)
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	if (ItemInfo->ItemCount == 1)
	{
		ItemInfo->ItemCount = 0;

		switch (ItemInfo->ID)
		{
		case EItemID::PT_HP_Potion:
			MainHUD->SetHPPotionCount(0);
			break;
		case EItemID::PT_MP_Potion:
			MainHUD->SetMPPotionCount(0);
			break;
		case EItemID::PT_Attack_Potion:
			MainHUD->SetAttackPotionCount(0);
			break;
		case EItemID::PT_Armor_Potion:
			MainHUD->SetArmorPotionCount(0);
			break;
		}
	}

	else
	{
		ItemInfo->ItemCount--;

		switch (ItemInfo->ID)
		{
		case EItemID::PT_HP_Potion:
			MainHUD->SetHPPotionCount(ItemInfo->ItemCount);
			break;
		case EItemID::PT_MP_Potion:
			MainHUD->SetMPPotionCount(ItemInfo->ItemCount);
			break;
		case EItemID::PT_Attack_Potion:
			MainHUD->SetAttackPotionCount(ItemInfo->ItemCount);
			break;
		case EItemID::PT_Armor_Potion:
			MainHUD->SetArmorPotionCount(ItemInfo->ItemCount);
			break;
		}
	}

	MainHUD->UpdatePotionCount(ItemInfo->ID);
}

void APlayerCharacter::SavePlayer()
{
	UPFSaveGame* SaveGame = NewObject<UPFSaveGame>();

	// Player Info
	SaveGame->mPlayerInfo = mPlayerInfo;
	SaveGame->mCameraZoomMin = mCameraZoomMin;
	SaveGame->mCameraZoomMax = mCameraZoomMax;
	SaveGame->mHPRatio = mHPRatio;
	SaveGame->mMPRatio = mMPRatio;
	SaveGame->mExpRatio = mExpRatio;

	// Potion Count
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	int32 HPPotionCount, MPPotionCount, AttackPotionCount, ArmorPotionCount;

	FItemDataInfo* Potion = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_HP_Potion);
	HPPotionCount = Potion->ItemCount;

	Potion = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_MP_Potion);
	MPPotionCount = Potion->ItemCount;

	Potion = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_Attack_Potion);
	AttackPotionCount = Potion->ItemCount;

	Potion = UInventoryManager::GetInst(GetWorld())->GetItemInfo(EItemID::PT_HP_Potion);
	ArmorPotionCount = Potion->ItemCount;

	SaveGame->mHPPotionCount = Potion->ItemCount;
	SaveGame->mMPPotionCount = Potion->ItemCount;
	SaveGame->mAttackPotionCount = Potion->ItemCount;
	SaveGame->mArmorPotionCount = Potion->ItemCount;

	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();

	mEquipedWeaponIndex = -1;
	mEquipedArmorIndex = -1;
	mEquipedAccesaryIndex = -1;

	if (IsValid(TileView))
	{
		if (mEquipedItem.Weapon)
			mEquipedWeaponIndex = TileView->GetIndexForItem(mEquipedItem.Weapon);

		if (mEquipedItem.Armor)
			mEquipedArmorIndex = TileView->GetIndexForItem(mEquipedItem.Armor);

		if (mEquipedItem.Accesary)
			mEquipedAccesaryIndex = TileView->GetIndexForItem(mEquipedItem.Accesary);
	}

	SaveGame->mEquipedWeaponIndex = mEquipedWeaponIndex;
	SaveGame->mEquipedArmorIndex = mEquipedArmorIndex;
	SaveGame->mEquipedAccesaryIndex = mEquipedAccesaryIndex;

	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Save"), 0);

	// Player Info
	GameMode->GetSaveGame()->mPlayerInfo = mPlayerInfo;
	GameMode->GetSaveGame()->mCameraZoomMin = mCameraZoomMin;
	GameMode->GetSaveGame()->mCameraZoomMax = mCameraZoomMax;
	GameMode->GetSaveGame()->mHPRatio = mHPRatio;
	GameMode->GetSaveGame()->mMPRatio = mMPRatio;
	GameMode->GetSaveGame()->mExpRatio = mExpRatio;

	// Potion Count
	GameMode->GetSaveGame()->mHPPotionCount = HPPotionCount;
	GameMode->GetSaveGame()->mMPPotionCount = MPPotionCount;
	GameMode->GetSaveGame()->mAttackPotionCount = AttackPotionCount;
	GameMode->GetSaveGame()->mArmorPotionCount = ArmorPotionCount;

	// Equiped Item Index
	GameMode->GetSaveGame()->mEquipedWeaponIndex = mEquipedWeaponIndex;
	GameMode->GetSaveGame()->mEquipedArmorIndex = mEquipedArmorIndex;
	GameMode->GetSaveGame()->mEquipedAccesaryIndex = mEquipedAccesaryIndex;

	//// Equiped Item
	//UPFGameInstance* GameInst = GetWorld()->GetGameInstance<UPFGameInstance>();
	//GameInst->SetEquipedItem(mEquipedItem);
}

void APlayerCharacter::Respawn()
{
	mDeath = false;

	mAnimInst->SetPlayerAnimType(EPlayerAnimType::Ground);

	mPlayerInfo.HP = mPlayerInfo.HPMax;
	mPlayerInfo.MP = mPlayerInfo.MPMax;

	// UI
	mHPRatio = 1.f;
	mMPRatio = 1.f;
	mExpRatio = 0.f;

	UpdatePlayerInfoUI();
}

void APlayerCharacter::StartSkill()
{
}

void APlayerCharacter::PlaySkillCameraShake()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), mSkillCameraShake, GetActorLocation(), 100.f, 1500.f, 1.f, false);
}

void APlayerCharacter::PlayUltimateCameraShake()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), mUltimateCameraShake, GetActorLocation(), 100.f, 2000.f, 0.5f, false);
}

void APlayerCharacter::CameraZoomInOut()
{
	if (mCameraZoomIn)
	{
		mSpringArm->TargetArmLength -= mCameraZoomSpeed;

		if (mSpringArm->TargetArmLength < mSkillCameraZoomMin)
		{
			mSpringArm->TargetArmLength = mSkillCameraZoomMin;
			mCameraZoom = false;
		}
	}

	else
	{
		if (mSpringArm->TargetArmLength < mSkillCameraZoomMax)
		{
			mSpringArm->TargetArmLength += mCameraZoomSpeed;
			mCameraZoom = false;
		}
	}
}

void APlayerCharacter::RecoveryMP(float DeltaTime)
{
	if (mPlayerInfo.MP < mPlayerInfo.MPMax)
	{
		mCurMPTime += DeltaTime;

		if (mCurMPTime >= mMPRecoveryTime)
		{
			PrintViewport(1.f, FColor::Red, TEXT("MP Recovery"));

			float MPRecovery = mPlayerInfo.MPMax * 0.1f;

			mPlayerInfo.MP += (int32)MPRecovery;

			// UI
			APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			UMainHUDBase* MainHUD = GameMode->GetMainHUD();

			mMPRatio = (float)mPlayerInfo.MP / mPlayerInfo.MPMax;
			MainHUD->SetMP(mMPRatio);
			MainHUD->SetPlayerStatMP(mPlayerInfo.MP);

			mCurMPTime = 0.f;
		}
	}
}

void APlayerCharacter::AttackBuffOn(float DeltaTime)
{
	if (!mAttackBuff.bBuffOn)
		return;

	mAttackBuff.CurBuffTime += DeltaTime;

	if (mAttackBuff.CurBuffTime <= mAttackBuff.BuffTime)
		mPlayerInfo.AttackPoint = mAttackBuff.OriginStat + mAttackBuff.BuffValue;

	else
	{
		APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();

		MainHUD->SetAttackBuffOff(mAttackBuff.BuffValue);

		mAttackBuff.bBuffOn = false;
		mAttackBuff.CurBuffTime = 0.f;
		mPlayerInfo.AttackPoint = mAttackBuff.OriginStat;
	}
}

void APlayerCharacter::ArmorBuffOn(float DeltaTime)
{
	if (!mArmorBuff.bBuffOn)
		return;

	mArmorBuff.CurBuffTime += DeltaTime;

	if (mArmorBuff.CurBuffTime <= mArmorBuff.BuffTime)
	{
		mPlayerInfo.ArmorPoint = mArmorBuff.OriginStat + mArmorBuff.BuffValue;
	}

	else
	{
		APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();

		MainHUD->SetArmorBuffOff(mArmorBuff.BuffValue);

		mArmorBuff.bBuffOn = false;
		mArmorBuff.CurBuffTime = 0.f;
		mPlayerInfo.ArmorPoint = mArmorBuff.OriginStat;
	}
}

void APlayerCharacter::EquipItem(UObject* Item)
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	FItemDataInfo* ItemInfo = Cast<UItemDataBase>(Item)->GetItemInfo();

	UItemDataBase* ItemDataBase = Cast<UItemDataBase>(Item);
	UInventoryItemBase* InventoryItemBase = Cast<UInventoryItemBase>(ItemDataBase);

	if (IsValid(InventoryItemBase))
	{
		InventoryItemBase->SetEquip();
	}

	// 아이템 장착
	switch (ItemInfo->ItemType)
	{
	case EItemType::Equip_Weapon:
		if (mEquipedItem.Weapon)
			return;

		mEquipedItem.Weapon = Item;
		mPlayerInfo.AttackPoint += ItemInfo->AttackBuff;

		MainHUD->SetEquipWeaponImage(ItemInfo->IconPath);
		MainHUD->SetAttackBuffText(ItemInfo->AttackBuff);
		break;
	case EItemType::Equip_Armor:
		if (mEquipedItem.Armor)
			return;

		mEquipedItem.Armor = Item;
		mPlayerInfo.ArmorPoint += ItemInfo->ArmorBuff;

		MainHUD->SetEquipArmorImage(ItemInfo->IconPath);
		MainHUD->SetArmorBuffText(ItemInfo->ArmorBuff);
		break;
	case EItemType::Equip_Accesary:
		if (mEquipedItem.Accesary)
			return;

		mEquipedItem.Accesary = Item;
		mPlayerInfo.AttackPoint += ItemInfo->AttackBuff;
		mPlayerInfo.ArmorPoint += ItemInfo->ArmorBuff;

		MainHUD->SetEquipAccesaryImage(ItemInfo->IconPath);
		MainHUD->SetAttackBuffText(ItemInfo->AttackBuff);
		MainHUD->SetArmorBuffText(ItemInfo->ArmorBuff);
		break;
	}
}

void APlayerCharacter::UnEquipItem(UObject* Item)
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	FItemDataInfo* ItemInfo = Cast<UItemDataBase>(Item)->GetItemInfo();

	switch (ItemInfo->ItemType)
	{
	case EItemType::Equip_Weapon:
		if (!mEquipedItem.Weapon)
			return;
		mPlayerInfo.AttackPoint -= ItemInfo->AttackBuff;

		MainHUD->SetUnEquipWeapon(ItemInfo->AttackBuff);

		mEquipedItem.Weapon = nullptr;
		break;
	case EItemType::Equip_Armor:
		if (!mEquipedItem.Armor)
			return;

		mPlayerInfo.ArmorPoint -= ItemInfo->ArmorBuff;

		MainHUD->SetUnEquipArmor(ItemInfo->ArmorBuff);

		mEquipedItem.Armor = nullptr;
		break;
	case EItemType::Equip_Accesary:
		if (!mEquipedItem.Accesary)
			return;

		mPlayerInfo.AttackPoint -= ItemInfo->AttackBuff;
		mPlayerInfo.ArmorPoint -= ItemInfo->ArmorBuff;

		MainHUD->SetUnEquipAccesary(ItemInfo->AttackBuff, ItemInfo->ArmorBuff);

		mEquipedItem.Accesary = nullptr;

		break;
	}

}

void APlayerCharacter::SetExp(float Ratio)
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	mExpRatio += Ratio;

	// Level Up 추가
	if (mExpRatio >= 1.f)
	{
		LevelUp();

		// PlayerInfo UI
		MainHUD->SetHP(mHPRatio);
		MainHUD->SetMP(mMPRatio);
		MainHUD->SetExp(mExpRatio);
		MainHUD->SetLevel(mPlayerInfo.Level);

		// PlayerStat UI
		MainHUD->SetPlayerStatAttackPoint(mPlayerInfo.AttackPoint);
		MainHUD->SetPlayerStatArmorPoint(mPlayerInfo.ArmorPoint);
		MainHUD->SetPlayerStatHP(mPlayerInfo.HP);
		MainHUD->SetPlayerStatMP(mPlayerInfo.MP);
		MainHUD->SetPlayerStatHPMax(mPlayerInfo.HPMax);
		MainHUD->SetPlayerStatMPMax(mPlayerInfo.MPMax);
	}

	else
		MainHUD->SetExp(Ratio);
}

void APlayerCharacter::SetGold(int Gold)
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	mPlayerInfo.Gold += Gold;

	MainHUD->SetGold(mPlayerInfo.Gold);
}

