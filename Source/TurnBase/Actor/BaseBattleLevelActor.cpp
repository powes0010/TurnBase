// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseBattleLevelActor.h"
#include "Character/TurnBaseCharacter.h"
#include "Character/BaseBattlePawn/BaseBattlePawn.h"
#include "Character/BaseEnemy/BaseEnemy.h"
#include "Controller/TurnBasePlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "UI/UIHPWidget.h"
#include "UI/UIBattleWidget.h"

const int32  SeatingNum = 6;



// Sets default values
ABaseBattleLevelActor::ABaseBattleLevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LevelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelMesh"));
	LevelMesh->SetRelativeLocation(FVector(0.f, 0.f, -5.f));
	LevelMesh->SetRelativeScale3D(FVector(4.f));
	LevelMesh->SetupAttachment(RootComponent);


	BattleLevelCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BattleLevelCamera"));
	BattleLevelCamera->SetupAttachment(RootComponent);
	BattleLevelCamera->SetRelativeRotation(FRotator(-30.f, -50.f, 0.f));
	BattleLevelCamera->SetRelativeLocation(FVector(-400.f, 380.f, 400.f));

	LevelType = EBattleLevelType::EBT_BiWuTai;

	RedArrowLoc.Add(FVector(-150.f, 0.f, 10.f));
	RedArrowLoc.Add(FVector(-150.f, -200.f, 10.f));
	RedArrowLoc.Add(FVector(-150.f, 200.f, 10.f));
	RedArrowLoc.Add(FVector(-350.f, 0.f, 10.f));
	RedArrowLoc.Add(FVector(-350.f, -200.f, 10.f));
	RedArrowLoc.Add(FVector(-350.f, 200.f, 10.f));

	BlueArrowLoc.Add(FVector(150.f, 0.f, 10.f));
	BlueArrowLoc.Add(FVector(150.f, -200.f, 10.f));
	BlueArrowLoc.Add(FVector(150.f, 200.f, 10.f));
	BlueArrowLoc.Add(FVector(350.f, 0.f, 10.f));
	BlueArrowLoc.Add(FVector(350.f, -200.f, 10.f));
	BlueArrowLoc.Add(FVector(350.f, 200.f, 10.f));

	for (int32 i = 0; i < RedArrowLoc.Num(); i++)
	{
		UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(*FString::Printf(TEXT("RedArrow_%d"), i));
		arrow->SetupAttachment(RootComponent);
		arrow->SetRelativeLocation(RedArrowLoc[i]);
		RedGroup.Add(arrow);
	}
	for (int32 i = 0; i < BlueArrowLoc.Num(); i++)
	{
		UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(*FString::Printf(TEXT("BlueArrow_%d"), i));
		arrow->SetupAttachment(RootComponent);
		arrow->SetRelativeLocation(BlueArrowLoc[i]);
		arrow->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		BlueGroup.Add(arrow);
	}


	
}

// Called when the game starts or when spawned
void ABaseBattleLevelActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!BattleUI)
	{
		UClass* BattleUIClass = LoadClass<UUIBattleWidget>(NULL, TEXT("/Game/Blueprint/UI/BPW_BattleWidget.BPW_BattleWidget_C"));
		if (BattleUIClass)
		{
			BattleUI = CreateWidget<UUIBattleWidget>(GetWorld(), BattleUIClass);
			if (BattleUI)
			{
				BattleUI->InitLevelActor(this);
			}
		}
	}
}

// Called every frame
void ABaseBattleLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BattleTick(DeltaTime);
	DoEnemySelect();
}

void ABaseBattleLevelActor::DoEnemySelect()
{
	if (bIsInEnemySelect)
	{
		ATurnBasePlayerController* TSC = Cast<ATurnBasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (TSC)
		{
			FVector StartTrace = FVector::ZeroVector;
			FRotator UnusedRot;
			TSC->GetPlayerViewPoint(StartTrace, UnusedRot);

			FVector MouseLoc = FVector::ZeroVector;
			FVector MouseDir = FVector::ZeroVector;
			bool bSuc = TSC->DeprojectMousePositionToWorld(MouseLoc, MouseDir);
			if (bSuc)
			{
				FHitResult result;
				FCollisionObjectQueryParams Params(ECC_Pawn);
				GetWorld()->LineTraceSingleByObjectType(result, StartTrace, StartTrace + MouseDir * 1000.f, Params);

				if (result.bBlockingHit)
				{
					ABaseBattlePawn* target = Cast<ABaseBattlePawn>(result.GetActor());
					if (target && CurSelectEnemy != target)
					{
						if (EnemyPawns.Contains(target))
						{
							if (CurSelectEnemy && CurSelectEnemy->GetHPWidget())
							{
								CurSelectEnemy->GetHPWidget()->SetSelectedImageVisible(false);
							}
							CurSelectEnemy = target;
							if (target->GetHPWidget())
							{
								target->GetHPWidget()->SetSelectedImageVisible(true);
							}
						}
					}
				}
			}
		}
	}	
}

void ABaseBattleLevelActor::BattleStart(class ATurnBaseCharacter* player, class ABaseEnemy* enemy)
{
	if (player && enemy)
	{
		ATurnBasePlayerController* TSC = Cast<ATurnBasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (TSC)
		{
			TSC->CurBattleLevelActor = this;

			Player = player;
			Enemy = enemy;

			bIsInBattleTick = true;
			TSC->bIsInBattleLevel = true;

			if (BattleUI)
			{
				BattleUI->AddToViewport();
			}

			for (int32 i = 0; i < FMath::Min<int32>(Player->GetPartners().Num(), SeatingNum); i++)
			{
				if (Player->GetPartners()[i].PartnerClass && RedGroup.Num() > i && RedGroup[i])
				{
					UClass* pawnclass = Player->GetPartners()[i].PartnerClass;
					ABaseBattlePawn* btp = GetWorld()->SpawnActor<ABaseBattlePawn>(pawnclass, RedGroup[i]->GetComponentTransform());
					if (btp)
					{
						PlayerPawns.Add(btp, FPawnMsg(i));
					}
				}
			}

			for (int32 i = 0; i < FMath::Min<int32>(Enemy->GetAllEnemy().Num(), SeatingNum); i++)
			{
				if (Enemy->GetAllEnemy()[i] && BlueGroup.Num() > i && BlueGroup[i])
				{
					UClass* pawnclass = Enemy->GetAllEnemy()[i];
					ABaseBattlePawn* btp = GetWorld()->SpawnActor<ABaseBattlePawn>(pawnclass, BlueGroup[i]->GetComponentTransform());
					if (btp)
					{
						EnemyPawns.Add(btp, FPawnMsg(i));
					}
				}
			}

			TSC->SetViewTarget(this);
		}
	}
}

void ABaseBattleLevelActor::BattleEnd(class ATurnBaseCharacter* player, class ABaseEnemy* enemy)
{

	PlayerPawns.Empty();
	EnemyPawns.Empty();
}

void ABaseBattleLevelActor::BattleTick(float DeltaTime)
{
	if (bIsInBattleTick)
	{
		for ( auto It = PlayerPawns.CreateIterator(); It ; ++It)
		{
			It.Value().FightSeq += It.Key()->Speed * DeltaTime;
			if (It.Value().FightSeq > 100.f)
			{
				bIsInBattleTick = false;
				PlayerSelectEnemy(It.Key());
				
				break;
			}
		}
	}
	if (bIsInBattleTick)
	{
		
	}

}

void ABaseBattleLevelActor::PlayerSelectEnemy(class ABaseBattlePawn* Attacker)
{
	if (Attacker)
	{
		CurAttacker = Attacker;
		if (Attacker->GetHPWidget())
		{
			Attacker->GetHPWidget()->SetArrowVisible(true);
		}
		if (BattleUI)
		{
			BattleUI->SetAttackOrderVisible(true);
		}
	}
}

void ABaseBattleLevelActor::PlayerDoAttack()
{
	if (CurAttacker && CurSelectEnemy && bIsInEnemySelect)
	{
		bIsInEnemySelect = false;
		if (BattleUI)
		{
			BattleUI->SetAttackOrderVisible(false);
		}
		DoAttack(CurAttacker, CurSelectEnemy, true);

	}
}

void ABaseBattleLevelActor::DoAttack(ABaseBattlePawn* Attacker, ABaseBattlePawn* Target, bool bIsPlayerAtk)
{
	UArrowComponent* TarArrow = bIsPlayerAtk ? BlueGroup[EnemyPawns[Target].LocIndex] : RedGroup[PlayerPawns[Target].LocIndex];
	if (TarArrow)
	{
		FVector TarLoc = TarArrow->GetComponentLocation() + TarArrow->GetForwardVector() * 50.f;

	}
}