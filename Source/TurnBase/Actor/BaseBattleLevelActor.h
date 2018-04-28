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

UCLASS()
class TURNBASE_API ABaseBattleLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBattleLevelActor();

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return BattleLevelCamera; }
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

	class ATurnBaseCharacter* Player = nullptr;

	class ABaseEnemy* Enemy = nullptr;

	UPROPERTY()
		TMap<class ABaseBattlePawn*, FPawnMsg> PlayerPawns;

	UPROPERTY()
		TMap<class ABaseBattlePawn*, FPawnMsg> EnemyPawns;

	bool bIsInBattleTick = false;

	UPROPERTY()
		UUIBattleWidget* BattleUI;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BattleTick(float DeltaTime);

	UPROPERTY(EditAnywhere)
		EBattleLevelType LevelType;

	void BattleStart(class ATurnBaseCharacter* player, class ABaseEnemy* enemy);

	void BattleEnd(class ATurnBaseCharacter* player, class ABaseEnemy* enemy);

protected:
	void PlayerDoAttack(class ABaseBattlePawn* Attacker);
	
};
