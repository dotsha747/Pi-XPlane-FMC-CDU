/*
 * CharsetTextures.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: shahada
 */

#include <sstream>
#include <stdexcept>
#include <iostream>

#include "CharsetTextures.h"

using namespace std;

CharsetTextures::CharsetTextures(std::string fontPath, SDL_Window * window,
		SDL_Renderer * renderer, int cellWidth, int cellHeight,
		SDL_Color fgColor, SDL_Color bgColor) {

	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;

	// create a big surface to hold all the characters we may ever need,
	// making it the same pixelformat as the screen.
	// Initalize Color Masks.
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	    int redMask   = 0xff000000;
	    int greenMask = 0x00ff0000;
	    int blueMask  = 0x0000ff00;
	    int alphaMask = 0x000000ff;
	#else
	    int redMask   = 0x000000ff;
	    int greenMask = 0x0000ff00;
	    int blueMask  = 0x00ff0000;
	    int alphaMask = 0xff000000;
	#endif
	int w = (96 - 32) * cellWidth;
	SDL_Surface * allCharSurface = SDL_CreateRGBSurface(0, w, cellHeight,
			32, redMask, greenMask, blueMask, alphaMask);
	if (allCharSurface == NULL) {
		cerr << "SDL_CreateRGBSurface failed for allCharSurface: "
		<< SDL_GetError() << endl;
		exit(1);
	}
	cerr << "allCharSurface is w:" << w <<  " h:" << cellHeight << endl;


	// paint the whole surface the background color
	SDL_FillRect(allCharSurface, NULL,
			SDL_MapRGB(allCharSurface->format, 0, 0, 0));
	SDL_SaveBMP(allCharSurface, "/tmp/z.bmp");


	// get a font of the appropriate size to fit our cell width and height
	TTF_Font * font = getFont(fontPath, cellWidth, cellHeight, renderer);


	// now let's render printable characters
	for (char c = 32; c < 96; c++) {

		// make a Unicode string
		Uint16 uc[2];
		if (c == '=') {
			// interpret equals as degree symbol
			uc[0] = 0x00b0;

		} else {
			uc[0] = c;
		};
		uc[1] = 0;

		// render that character to it's own surface
		SDL_Surface* shaded = TTF_RenderUNICODE_Shaded(font,
				(const Uint16 *) uc, fgColor, bgColor);

		if (c == 35) {
			SDL_SaveBMP(shaded, "/tmp/hash.bmp");
		}

		// work out horizontal offset to center this char within a cell
		int hOffset = (cellWidth - shaded->w) / 2;

		// copy the character surface into allCharSurface
		SDL_SetSurfaceBlendMode(shaded, SDL_BLENDMODE_NONE);
		SDL_Rect dstrect = { ((c - 32) * cellWidth) + hOffset, 0, shaded->w,
				shaded->h };
		SDL_BlitSurface(shaded, NULL, allCharSurface, &dstrect);

		cerr << "Char " << (int) c << " [" << c << "] rendered width is w:"
				<< shaded->w << " h:" << shaded->h << " at allCharSet ["
				<< dstrect.x << ", " << dstrect.y << ", " << dstrect.w << ", "
				<< dstrect.h << "]" << endl;

		// release the shadedSurface
		SDL_FreeSurface(shaded);
	}

	// release the font
	TTF_CloseFont(font);



	// convert allCharSurface to allCharTexture
	allCharTexture = SDL_CreateTextureFromSurface(renderer, allCharSurface);
	SDL_FreeSurface(allCharSurface);

}




CharsetTextures::~CharsetTextures() {

	SDL_DestroyTexture(allCharTexture);

}




/** @brief figure out the ideal font point size that matches our height
 *
 * 	http://stackoverflow.com/questions/5829703/java-getting-a-font-with-a-specific-height-in-pixels
 *
 */

TTF_Font * CharsetTextures::getFont(std::string fontPath, int cellWidth,
		int cellHeight, SDL_Renderer * renderer) {

	// cache these values
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;

	if (fontPath.empty()) {
		fontPath = "RobotoMono-Regular.ttf";
	}

	TTF_Font * font = NULL;
	int size = cellHeight;
	int dir = 0;
	int testHeight;

	while (true) {

		if (font != NULL) {
			TTF_CloseFont(font);
		}

		font = TTF_OpenFont(fontPath.c_str(), size);
		if (font == NULL) {
			ostringstream buf;
			buf << "SDL2_TTF could not load font from \"" << fontPath
					<< "\" size " << size << "pt, SDL_Error: "
					<< SDL_GetError();
			throw runtime_error(buf.str());
		}

		// Render the character "W" and get the width.
		SDL_Color color = { 255, 255, 255 };
		SDL_Color bgcolor = { 0, 0, 0 };
		SDL_Surface * surface = TTF_RenderText_Shaded(font, "W", color,
				bgcolor);
		SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
		int texW = 0;
		int texH = 0;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		SDL_Rect dstrect = { 0, 0, texW, texH };
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		testHeight = TTF_FontHeight(font);

		cerr << "Font size " << size << "pt has width:" << texW
				<< " and height " << testHeight << endl;

		if ((testHeight < cellHeight && texW < cellWidth) && dir != -1) {
			size++;
			dir = 1;
		} else if ((testHeight > cellHeight || texW > cellWidth) && dir != 1) {
			size--;
			dir = -1;
		} else {
			break;
		}
	}

	cerr << "Using font \"" << fontPath << "\" size " << size << "pt, height = "
			<< testHeight << " to fill row of " << cellHeight << endl;

	return font;
}




/** @brief return the texture for a given character
 *
 */

void CharsetTextures::renderCharAt(SDL_Renderer * renderer, char c, int x,
		int y) {

	if (islower(c)) {
		c = toupper(c);
	}

	// figure out the area of the character in allCharTexture
	SDL_Rect srcrect = { (c - 32) * cellWidth, 0, cellWidth, cellHeight };

	// the destination in the screen
	SDL_Rect dstrect = { x, y, cellWidth, cellHeight };

	SDL_RenderCopy(renderer, allCharTexture, &srcrect, &dstrect);
	SDL_RenderPresent(renderer);

	cerr << "RenderCopy char " << (int) c << " [" << c << "] from allCharSet ["
			<< srcrect.x << ", " << srcrect.y << ", " << srcrect.w << ", "
			<< srcrect.h << "] to screen at [" << dstrect.x << ", " << dstrect.y
			<< ", " << dstrect.w << ", " << dstrect.h << "]" << endl;

}
