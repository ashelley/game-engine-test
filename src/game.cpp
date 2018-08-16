#ifndef GAME_H
#define GAME_H

#include "defines.h"
#include "vec3.h"

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

static void DrawRectangleDumbly(int x0, int y0, int x1, int y1, offscreen_buffer *Buffer, vec3 color) {
	u8 *Row = (u8 *)Buffer->Memory;
	int Pitch = Buffer->BytesPerPixel * Buffer->Width;

	// int testY0 = y0;	
	// int testY1 = y1;

	int testY0 = Buffer->Height - y1;
	int testY1 = Buffer->Height - y0;	
	
	int testX0 = x0;
	int testX1 = x1;		

	for (int Y = 0; Y < Buffer->Height; Y++) {
		u32 *Pixel = (u32 *)Row;
		for (int X = 0; X < Buffer->Width; X++) {
			if(X >= testX0 && X < testX1 && Y >= testY0 && Y < testY1) {
				u8 R = color.r();
				u8 G = color.g();
				u8 B = color.b();

				u32 Color = ((B << 0) | (G <<8) | (R << 16)) /* | (A << 24)*/;
				*Pixel = Color;				
			}
			++Pixel;
		}
		Row += Pitch;
	}
}

static void DrawLineWithoutCalculatingNumberPixelsNeeded(int x0, int y0, int x1, int y1, offscreen_buffer *Buffer, vec3 color) {

	u32 *StartAt = (u32 *)Buffer->Memory + (Buffer->Width * Buffer->Height) - Buffer->Width; // start at bottom corner as 0,0

	for(float t=0.0; t < 1.0; t += 0.1) {
		int x = (x0 * (1.0 - t)) + (x1 * t);
		int y = (y0 * (1.0 - t)) + (y1 * t);

		u8 R = color.r();
		u8 G = color.g();
		u8 B = color.b();

		u32 *Pixel = StartAt + x - (y * Buffer->Width); // start of memory is the top off the buffer but we are drawing bottom to top so we substract y
		*Pixel = ((B << 0) | (G << 8) | (R<< 16));

	}
}

static float AbsoluteValue(float a) {
	if (a < 0) {
		return a * -1;
	}
	return a;
}

static int AbsoluteValue(int a) {
	if(a < 0) {
		return a * -1;
	}
	return a;
}

static void Swap(int *a, int *b) {
	int old = *b;
	*b = *a;
	*a = old;
}

static void DrawLine(int x0, int y0, int x1, int y1, offscreen_buffer *Buffer, vec3 color) {

	u32 *StartAt = (u32 *)Buffer->Memory + (Buffer->Width * Buffer->Height) - Buffer->Width; // start at bottom corner as 0,0

	bool steep = false;

	if(AbsoluteValue(x0-x1) < AbsoluteValue(y0-y1)) {
		steep = true;
		Swap(&x0, &y0);
		Swap(&x1, &y1);
	}

	if(x0 > x1) {
		Swap(&x0, &x1);
		Swap(&y0, &y1);
	}

	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;

		u8 R = color.r();
		u8 G = color.g();
		u8 B = color.b();

		u32 *Pixel = StartAt;
		if(steep) {
			Pixel += y - (x * Buffer->Width); // this is just cause we stopped y and x;
		}
		else {
			Pixel += x - (y * Buffer->Width); // start of memory is the top off the buffer but we are drawing bottom to top so we substract y
		}
		 
		*Pixel = ((B << 0) | (G << 8) | (R << 16));


	}
}


static void GameUpdateAndRender(offscreen_buffer *Buffer) {
    RenderWeirdGradient(Buffer, 0, 0);

	vec3 white = vec3(255, 255, 255);
	vec3 color0 = vec3(0, 0, 255);
	vec3 color1 = vec3(255,0,0);
	vec3 color2 = vec3(255, 0, 255);
	vec3 color3 = vec3(255, 255, 0);

	DrawLineWithoutCalculatingNumberPixelsNeeded(0, 0, 100, 100, Buffer, color0);
	
	DrawLine(0, 200, 200, 250, Buffer, color1);
	DrawLine(0, 300, 25, 500, Buffer, color2);

	DrawRectangleDumbly(0, 200, 10, 210, Buffer, white);
	DrawRectangleDumbly(200, 250, 210, 260, Buffer, white);

	DrawRectangleDumbly(0, 300, 10, 310, Buffer, white);
	DrawRectangleDumbly(25, 500, 35, 510, Buffer, white);


	DrawRectangleDumbly(200,200,250,250, Buffer, color3);
}

#endif
