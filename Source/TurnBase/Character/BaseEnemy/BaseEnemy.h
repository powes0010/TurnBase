// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class TURNBASE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class ABaseBattlePawn>> Enemys;

	UPROPERTY(BlueprintReadOnly)
		bool bIsAlive;
public:
	FORCEINLINE bool GetIsAlive() { return bIsAlive; }

	FORCEINLINE TArray<TSubclassOf<class ABaseBattlePawn>>& GetAllEnemy() { return Enemys; }


	
	
};
