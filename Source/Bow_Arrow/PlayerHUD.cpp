// Fill out your copyright notice in the Description page of Project Settings.

#include "Bow_Arrow.h"
#include "PlayerHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D>CrosshairTexObj(TEXT("Texture2D'/Game/MyAssets/Textures/FirstPersonCrosshair.FirstPersonCrosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
}
void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
	if (CrosshairTex)
	{
		DrawCrosshair();
	}
}

void APlayerHUD::DrawCrosshair()
{
	float TextureWidth = CrosshairTex->GetSurfaceWidth();
	float TextureHeight = CrosshairTex->GetSurfaceHeight();

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY* 0.5f);
	const FVector2D CrosshairDrawPosition(Center.X - TextureWidth * 0.5f, Center.Y - TextureHeight * 0.5f);

	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}
