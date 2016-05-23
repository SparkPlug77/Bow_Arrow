// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class BOW_ARROW_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	APlayerHUD();
	virtual void DrawHUD() override;
	
private:
	class UTexture2D* CrosshairTex;
	void DrawCrosshair();
	
};
