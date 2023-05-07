// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"	// 디버깅용 도형 출력기능.
#include "EngineGlobals.h"
#include "Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"
#include "UObject/NoExportTypes.h"
#include "GameInfo.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PF, Log, All);

#define	LOG_CALLINFO		(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

#define LOG(Format, ...)	UE_LOG(PF, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOGSTRING(Str)		UE_LOG(PF, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

void PrintViewport(float Time, const FColor& Color, const FString& Text);



USTRUCT(BlueprintType)
struct FPotionBuff
{
	GENERATED_USTRUCT_BODY()

public:
	bool		bBuffOn;
	float		CurBuffTime;
	float		BuffTime;
	float		BuffValue;
	float		OriginStat;
};

USTRUCT(BlueprintType)
struct FEquipedItem
{
	GENERATED_USTRUCT_BODY()

public:
	UObject* Weapon;
	UObject* Armor;
	UObject* Accesary;
};

UENUM(BlueprintType)
enum class EPlayerJob : uint8
{
	None,
	Knight,
	Archer,
	Magicion,
	Summoner,
	End
};

UENUM(BlueprintType)
enum class EPlayerSkillKey : uint8
{
	SkillQ,
	SkillE,
	SkillR,
	SkillRM
};

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName		Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		AttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		ArmorPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		HPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		MPMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		MoveSpeed;
};

USTRUCT(BlueprintType)
struct FPlayerInfo : public FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		ExpMax;
};

USTRUCT(BlueprintType)
struct FMonsterInfo : public FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		TraceDistance;
};

UENUM(BlueprintType)
enum class EMonsterAnimType : uint8
{
	Idle,
	Run,
	Attack,
	Death,
	KnockBack,
	Stun,
	Skill1,
	Skill2,
	Skill3
};

UENUM(BlueprintType)
enum class EMonsterPhase : uint8
{
	NormalAttack,
	Skill1,
	Skill2,
	Skill3
};


UENUM(BlueprintType)
enum class EWolfAnimType : uint8
{
	Attack,		// SkillQ, SkillR
	Run			// SkillE
};

UENUM(BlueprintType)
enum class EWolfSkillType : uint8
{
	LeapAttack,
	RunAround
};

USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			SlotNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			SkillNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class ASkillActor*	SkillActor;
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Passive,
	Active
};

UENUM(BlueprintType)
enum class ESkillSystem : uint8
{
	Attack_Single_Once,
	Attack_Single_Duration,
	Attack_Multi_Once,
	Attack_Multi_Duration,
	Buf_Single,
	Buf_Multi
};

UENUM(BlueprintType)
enum class ESkillOptionType : uint8
{
	Damage,
	AttackUp,
	ArmorUp,
	HPRecovery,
	MPRecovery
};

USTRUCT(Atomic, BlueprintType)
struct FSkillOption
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillOptionType		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		Option;
};

UENUM(BlueprintType)
enum class ESkillEffectType : uint8
{
	Cast,
	Play,
	Trail
};

USTRUCT(Atomic, BlueprintType)
struct FSkillEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillEffectType		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraSystem*			Effect;
};

UENUM(BlueprintType)
enum class ESkillUseType : uint8
{
	HPPercent,
	Duration,
	Ratio
};

USTRUCT(Atomic, BlueprintType)
struct FSkillUseData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillUseType		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				Data;
};

USTRUCT(Atomic, BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillType		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillSystem	System;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString			SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		LimitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPlayerJob	LimitJob;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillOption>		SkillOptionArray;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//TArray<FSkillEffectData>		SkillEffectArray;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//TArray<FSkillUseData>			SkillUseDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float			Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			MP;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//EMonsterAnimType		AnimType;
};

USTRUCT(BlueprintType)
struct FMonsterSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillType			Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillSystem		System;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillOption>	SkillOptionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	Distance;
};

USTRUCT(BlueprintType)
struct FPlayerSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillType			Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkillSystem		System;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString				SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString				Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillOption>	SkillOptionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float			Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float			Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			MP;
};

USTRUCT(BlueprintType)
struct FShinbiSkillInfo : public FPlayerSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float					AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EWolfSkillType			WolfSkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32					WolfCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EWolfAnimType			WolfAnimType;
};

USTRUCT(BlueprintType)
struct FMonsterTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName				Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32				AttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32				ArmorPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32				HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32				MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32				Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32				Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32				Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMesh*		Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UAnimInstance>	MonsterAnimClass;

};

UENUM(BlueprintType)
enum class EPatrolEndDir : uint8
{
	Progress,
	Repeat
};

UENUM(BlueprintType)
enum class EPatrolType : uint8
{
	Point,
	Spline
};

UENUM(BlueprintType)
enum class ETriggerShape : uint8
{
	Box,
	Sphere,
	Capsule
};

// Item
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Potion,
	
	Equip_Weapon,
	Equip_Armor,
	Equip_Accesary,

	End
};

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	HP,
	MP,
	AttackBuff,
	ArmorBuff,
	End,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	GreatSwrod,
	LongBow,
	PoisionDagger,
	End
};

UENUM(BlueprintType)
enum class ArmorType : uint8
{
	LeatherArmor,
	KnightArmor,
	End
};

UENUM(BlueprintType)
enum class EAccesaryType : uint8
{
	Ring,
	Necklace,
	End
};

UENUM(BlueprintType)
enum class EItemID : uint8
{
	NONE,

	PT_HP_Potion,
	PT_MP_Potion, 
	PT_Attack_Potion,
	PT_Armor_Potion,

	EW_GreatSword,
	EW_LongBow,
	EW_PoisonDagger,

	EA_LeatherArmor,
	EA_KnightArmor,

	AC_Ring,
	AC_Necklace,

	End
};

USTRUCT(Atomic, BlueprintType)
struct FSaveItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EItemID			ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			ItemCount;
};

USTRUCT(Atomic, BlueprintType)
struct FItemDataInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EItemID			ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EItemType		ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString			ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString			Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString			IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32			ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float			HPHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float			MPHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float			AttackBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float			ArmorBuff;
};

UCLASS()
class PF_API UGameInfo : public UObject
{
	GENERATED_BODY()
	
};
