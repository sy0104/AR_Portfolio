// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "../Manager/InventoryManager.h"
#include "../PFGameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UMG/InventoryBase.h"
#include "../UMG/InventoryItemBase.h"
#include "../UMG/ItemDataBase.h"

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

	mHPRatio = 1.f;
	mMPRatio = 1.f;
	mExpRatio = 0.f;

	mAttackBuff.bBuffOn = false;
	mAttackBuff.CurBuffTime = 0.f;
	mAttackBuff.BuffTime = 10.f;

	mArmorBuff.bBuffOn = false;
	mArmorBuff.CurBuffTime = 0.f;
	mArmorBuff.BuffTime = 10.f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	UInventoryManager::GetInst(GetWorld())->ShowInventory(false);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("AttackPoint : %d"), mPlayerInfo.AttackPoint));

	AttackBuffOn(DeltaTime);
	ArmorBuffOn(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �׼� ����
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::NormalAttack);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillQ"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillQKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillE"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillEKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillR"), EInputEvent::IE_Released, this, &APlayerCharacter::SkillRKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("SkillF"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillFKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InventoryOn);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Stat"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PlayerStatOnOff);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Cheat"), EInputEvent::IE_Pressed, this, &APlayerCharacter::CheatKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Item"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ItemKey);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("HPPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::HPPotionkey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("MPPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::MPPotionKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("AttackPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::AttackPotionKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("ArmorPotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ArmorPotionKey);

	// �� ����
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraZ"), this, &APlayerCharacter::RotationCameraZ);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraY"), this, &APlayerCharacter::RotationCameraY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoom);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage = Damage - mPlayerInfo.ArmorPoint;
	Damage = Damage < 1 ? 1 : Damage;

	mPlayerInfo.HP -= Damage;

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Damage: %f"), Damage));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("HP: %d"), mPlayerInfo.HP));

	if (mPlayerInfo.HP <= 0)	// ����
	{
		APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();
		MainHUD->SetHP(0.f);
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

		else if (Scale == 1.f)	// ������ �̵�
			mAnimInst->SetMoveDir(45.f);

		else if (Scale == -1.f)	// ���� �̵�
			mAnimInst->SetMoveDir(-45.f);
	}

	else if (mMoveDir == -1.f)	// S
	{
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(180.f);

		else if (Scale == 1.f)	// ������ �̵�
			mAnimInst->SetMoveDir(135.f);

		else if (Scale == -1.f)	// ���� �̵�
			mAnimInst->SetMoveDir(-135.f);
	}

	else
	{
		if (Scale == 0.f)		// �ƹ� Ű�� ������ ���� ���
			mAnimInst->SetMoveDir(0.f);

		else if (Scale == 1.f)	// ������ �̵�
			mAnimInst->SetMoveDir(90.f);

		else if (Scale == -1.f)	// ���� �̵�
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

	mAnimInst->UseSkill(0);
}

void APlayerCharacter::SkillEKey()
{
	if (mDeath || mTargeting)
		return;

	mAnimInst->UseSkill(1);
}

void APlayerCharacter::SkillRKey()
{
	if (mDeath)
		return;

	mAnimInst->UseSkill(2);
}

void APlayerCharacter::SkillFKey()
{
	if (mDeath || mTargeting)
		return;

	PrintViewport(1.f, FColor::Red, TEXT("SkillFKey()"));

	mAnimInst->UseSkill(3);
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
	if (UInventoryManager::GetInst(GetWorld())->IsInventoryOpen())
		UInventoryManager::GetInst(GetWorld())->ShowInventory(false);
	else
		UInventoryManager::GetInst(GetWorld())->ShowInventory(true);
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

void APlayerCharacter::SkillF()
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

void APlayerCharacter::UsePotion(FItemDataInfo* ItemInfo)
{
	// ���� ������ - ����

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

void APlayerCharacter::AttackBuffOn(float DeltaTime)
{
	if (!mAttackBuff.bBuffOn)
		return;

	mAttackBuff.CurBuffTime += DeltaTime;

	if (mAttackBuff.CurBuffTime <= mAttackBuff.BuffTime)
	{

		mPlayerInfo.AttackPoint = mAttackBuff.OriginStat + mAttackBuff.BuffValue;
	}

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

	// ������ ����
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

	// Level Up �߰�
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
