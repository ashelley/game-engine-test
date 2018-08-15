#ifndef GAME_H
#define GAME_H

#include "defines.h"

struct offscreen_buffer {
    void* Memory;
    int Width;
    int Height;
    int BytesPerPixel = 4;
};

static void RenderWeirdGradient(offscreen_buffer *Buffer, int XOffset, int YOffset) {
	u8 *Row = (u8 *)Buffer->Memory;
	int Pitch = Buffer->BytesPerPixel * Buffer->Width;

	for (int Y = 0; Y < Buffer->Height; Y++) {
		u32 *Pixel = (u32 *)Row;
		for (int X = 0; X < Buffer->Width; X++) {

			// BB GG RR xx
			// *Pixel++ = X + XOffset;//(u8)X;
			// *Pixel++ = Y + YOffset;//(u8)Y;
			// *Pixel++ = 0;
			// *Pixel++ = 0;

            u8 R = 0;
            u8 G = (X + XOffset);
            u8 B = (Y + YOffset);
            //u8 A = 255;

            u32 Color = ((B << 0) | (G <<8) | (R << 16)) /* | (A << 24)*/;
            *Pixel++ = Color;
		}
		Row += Pitch;
	}
}

static void GameUpdateAndRender(offscreen_buffer *Buffer) {
    RenderWeirdGradient(Buffer, 0, 0);
}

#endif
