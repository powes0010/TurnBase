// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseBattlePawn.generated.h"

UCLASS()
class TURNBASE_API ABaseBattlePawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseBattlePawn();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UUIHPWidget* GetHPWidget() { return HPWidget; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* WidgetComComponent;

	class UUIHPWidget* HPWidget = nullptr;

public:
	UPROPERTY(EditDefaultsOnly)
		float HP = 100.f;
	UPROPERTY(EditDefaultsOnly)
		float MaxHP = 100.f;
	UPROPERTY(EditDefaultsOnly)
		float ATK = 10.f;
	UPROPERTY(EditDefaultsOnly)
		float Speed = 100.f;
	


};
