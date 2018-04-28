// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBattleLevelActor.generated.h"

UENUM(BlueprintType)
enum class EBattleLevelType : uint8
{
	EBT_BiWuTai  UMETA(DisplayName = "±»Œ‰Ã®")
};

USTRUCT(BlueprintType)
struct FPawnMsg
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float FightSeq;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		int32 LocIndex;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		bool bIsAlive;
	FPawnMsg():FightSeq(0.f), LocIndex(0), bIsAlive(true)
	{}
	FPawnMsg(int32 LIdex) :LocIndex(LIdex), FightSeq(0.f), bIsAlive(true)
	{}
};

class UUIBattleWidget;
class ABaseBattlePawn;

UCLASS()
class TURNBASE_API ABaseBattleLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBattleLevelActor();

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return BattleLevelCamera; }

	FORCEINLINE	void SetIsInEnemySelect(bool InBool) { bIsInEnemySelect = InBool; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* BattleLevelCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LevelMesh;

	UPROPERTY(EditAnywhere)
		TArray<FVector> RedArrowLoc;

	UPROPERTY(EditAnywhere)
		TArray<FVector> BlueArrowLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
		TArray<class UArrowComponent*> RedGroup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
		TArray<class UArrowComponent*> BlueGroup;

	UPROPERTY()
		UUIBattleWidget* BattleUI;

	class ATurnBaseCharacter* Player = nullptr;

	class ABaseEnemy* Enemy = nullptr;

	UPROPERTY()
		TMap<ABaseBattlePawn*, FPawnMsg> PlayerPawns;

	UPROPERTY()
		TMap<ABaseBattlePawn*, FPawnMsg> EnemyPawns;

	bool bIsInBattleTick = false;

	bool bIsInEnemySelect = false;

	

	ABaseBattlePawn* CurAttacker = nullptr;

	ABaseBattlePawn* CurSelectEnemy = nullptr;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BattleTick(float DeltaTime);

	UPROPERTY(EditAnywhere)
		EBattleLevelType LevelType;

	void BattleStart(class ATurnBaseCharacter* player, class ABaseEnemy* enemy);

	void BattleEnd(class ATurnBaseCharacter* player, class ABaseEnemy* enemy);

	void PlayerDoAttack();
protected:
	void PlayerSelectEnemy(ABaseBattlePawn* Attacker);
	
	void DoEnemySelect();

	void DoAttack(ABaseBattlePawn* Attacker, ABaseBattlePawn* Target, bool bIsPlayerAtk);
};
