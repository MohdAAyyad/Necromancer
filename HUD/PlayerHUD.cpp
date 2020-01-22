// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
	if (crosshairTexture && bDrawing)
	{
		// Find the center of our canvas.
		FVector2D Center(Canvas->ClipX * 0.47f, Canvas->ClipY * 0.55f);

		// Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
		FVector2D CrossHairDrawPosition(Center.X - (crosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (crosshairTexture->GetSurfaceHeight() * 0.5f));

		// Draw the crosshair at the centerpoint.
		FCanvasTileItem TileItem(CrossHairDrawPosition, crosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}