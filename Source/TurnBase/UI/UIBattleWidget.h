// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBattleWidget.generated.h"

/**
 * 
 */
class UButton;
class UVerticalBox;

UCLASS()
class TURNBASE_API UUIBattleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	FORCEINLINE void InitLevelActor(class ABaseBattleLevelActor* CurLevelActor) { LecelActor = CurLevelActor; }
protected:
	UButton * AtkBtn = nullptr;

	UButton* SkillBtn = nullptr;

	UButton* DefenseBtn = nullptr;

	UVerticalBox* AttackOrderPanel = nullptr;

	ABaseBattleLevelActor* LecelActor = nullptr;

protected:
	UFUNCTION()
		void OnAttackBthClick();

public:
	void SetAttackOrderVisible(bool NewVis);
};
