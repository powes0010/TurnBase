// Fill out your copyright notice in the Description page of Project Settings.

#include "UIHPWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUIHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NameTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTxt")));

	HPTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPTxt")));

	ArrowImage = Cast<UImage>(GetWidgetFromName(TEXT("Arrow")));
}

void UUIHPWidget::SetArrowVisible(bool NewVis)
{
	if (ArrowImage)
	{
		ArrowImage->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

