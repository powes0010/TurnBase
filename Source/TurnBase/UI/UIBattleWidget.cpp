// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBattleWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Controller/TurnBasePlayerController.h"
#include "Actor/BaseBattleLevelActor.h"

void UUIBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AtkBtn = Cast<UButton>(GetWidgetFromName(TEXT("Attack")));
	AtkBtn->OnClicked.AddDynamic(this, &UUIBattleWidget::OnAttackBthClick);

	SkillBtn = Cast<UButton>(GetWidgetFromName(TEXT("Skill")));
	DefenseBtn = Cast<UButton>(GetWidgetFromName(TEXT("Defense")));
	AttackOrderPanel = Cast<UVerticalBox>(GetWidgetFromName(TEXT("AttackOrder")));

}

void UUIBattleWidget::SetAttackOrderVisible(bool NewVis)
{
	if (AttackOrderPanel)
	{
		AttackOrderPanel->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIBattleWidget::OnAttackBthClick()
{
	if (LecelActor)
	{
		LecelActor->SetIsInEnemySelect(true);
	}
}