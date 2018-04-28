// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBattleWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"


void UUIBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AtkBtn = Cast<UButton>(GetWidgetFromName(TEXT("Attack")));
	SkillBtn = Cast<UButton>(GetWidgetFromName(TEXT("Skill")));
	DefenseBtn = Cast<UButton>(GetWidgetFromName(TEXT("Defense")));
	AttackOrderPanel = Cast<UVerticalBox>(GetWidgetFromName(TEXT("AttackOrder")));

}
