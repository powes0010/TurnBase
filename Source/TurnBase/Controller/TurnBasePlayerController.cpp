// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TurnBasePlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Character/TurnBaseCharacter.h"
#include "Engine/World.h"

ATurnBasePlayerController::ATurnBasePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATurnBasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ATurnBasePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATurnBasePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATurnBasePlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("ZoomOut", IE_Released, this, &ATurnBasePlayerController::OnZommOut);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &ATurnBasePlayerController::OnZoomIn);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATurnBasePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATurnBasePlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ATurnBasePlayerController::OnResetVR);
}

void ATurnBasePlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATurnBasePlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ATurnBaseCharacter* MyPawn = Cast<ATurnBaseCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ATurnBasePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ATurnBasePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 20.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ATurnBasePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	if (!bIsInBattleLevel)
	{
		bMoveToMouseCursor = true;
	}
}

void ATurnBasePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	if (!bIsInBattleLevel)
	{
		bMoveToMouseCursor = false;
		StopMovement();
	}
}
void ATurnBasePlayerController::OnZommOut()
{
	ATurnBaseCharacter* character = Cast<ATurnBaseCharacter>(GetPawn());
	if (character)
	{
		character->GetCameraBoom()->TargetArmLength = FMath::Min<float>(character->GetCameraBoom()->TargetArmLength + 10.f, 600.f);
	}
}

void ATurnBasePlayerController::OnZoomIn()
{
	ATurnBaseCharacter* character = Cast<ATurnBaseCharacter>(GetPawn());
	if (character)
	{
		character->GetCameraBoom()->TargetArmLength = FMath::Max<float>(character->GetCameraBoom()->TargetArmLength - 10.f, 200.f);
	}
}

