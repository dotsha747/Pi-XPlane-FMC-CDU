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

CharsetTextures::CharsetTextures(std::string fontPath, int fontHeightPx,
		SDL_Renderer * renderer) {

	TTF_Font * font = getFont(fontPath, fontHeightPx, renderer);

	SDL_Color textColor = { 0, 255, 0, 255 }; // white
	SDL_Color backgroundColor = { 0, 0, 0, 255 }; // black

	// let's start with 0-9
	for (char c = '0'; c <= '9'; c++) {
		string s(1, c);
		generateTextureForChar(s.c_str(), c - '0', font, renderer, textColor,
				backgroundColor);

	}

	// here's A-Z
	for (char c = 'A'; c <= 'Z'; c++) {
		string s(1, c);
		generateTextureForChar(s.c_str(), 10 + c - 'A', font, renderer,
				textColor, backgroundColor);
	}

	// here's the rest
	generateTextureForChar(" ", 38, font, renderer, textColor, backgroundColor);
	generateTextureForChar("<", 39, font, renderer, textColor, backgroundColor);
	generateTextureForChar("-", 40, font, renderer, textColor, backgroundColor);
	generateTextureForChar(">", 41, font, renderer, textColor, backgroundColor);
	generateTextureForChar("%", 42, font, renderer, textColor, backgroundColor);
	generateTextureForChar("°", 43, font, renderer, textColor, backgroundColor);
	generateTextureForChar("/", 44, font, renderer, textColor, backgroundColor);
	generateTextureForChar(";", 45, font, renderer, textColor, backgroundColor);
	generateTextureForChar(".", 46, font, renderer, textColor, backgroundColor);
	generateTextureForChar("+", 47, font, renderer, textColor, backgroundColor);
	generateTextureForChar("[", 48, font, renderer, textColor, backgroundColor);
	generateTextureForChar("]", 49, font, renderer, textColor, backgroundColor);
	generateTextureForChar("?", 50, font, renderer, textColor, backgroundColor);
	generateTextureForChar("(", 51, font, renderer, textColor, backgroundColor);
	generateTextureForChar(")", 52, font, renderer, textColor, backgroundColor);

	// release the font
	TTF_CloseFont(font);

}



void CharsetTextures::generateTextureForChar(std::string c, int index,
		TTF_Font * font, SDL_Renderer * renderer, SDL_Color textColor,
		SDL_Color backgroundColor) {

	SDL_Surface* shaded = TTF_RenderUTF8_Shaded(font, c.c_str(), textColor, backgroundColor);
	cout << "Char " << c << " rendered width is w:" << shaded->w << " h:" << shaded->h << endl;
	charTextures[index] = SDL_CreateTextureFromSurface(renderer, shaded);
	SDL_FreeSurface(shaded);

	/*
	 SDL_Rect rect;
	 SDL_QueryTexture(charTextures[index], NULL, NULL, &rect.w, &rect.h);
	 cout << "Rendered texture for \"" << c << "\" at " << rect.w << " X "
	 << rect.h << endl;
	 */
}



CharsetTextures::~CharsetTextures() {

	for (int i = 0; i <= 45; i++) {
		SDL_DestroyTexture(charTextures[i]);
	}

}



/** @brief figure out the ideal font point size that matches our height
 *
 * 	http://stackoverflow.com/questions/5829703/java-getting-a-font-with-a-specific-height-in-pixels
 *
 */

TTF_Font * CharsetTextures::getFont(std::string fontPath, int maxHeight,
		SDL_Renderer * renderer) {


	if (fontPath.empty()) {
		fontPath = "/home/shahada/.fonts/RobotoMono-Regular.ttf";
	}

	TTF_Font * font = NULL;
	int size = maxHeight;
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

		testHeight = TTF_FontHeight(font);
		if (testHeight < maxHeight && dir != -1) {
			size++;
			dir = 1;
		} else if (testHeight > maxHeight && dir != 1) {
			size--;
			dir = -1;
		} else {
			break;
		}

		size--;

	}

	cout << "Loaded font \"" << fontPath << "\" size " << size
			<< "pt, height = " << testHeight << endl;

	return font;
}


/** @brief return the texture for a given character
 *
 */

SDL_Texture * CharsetTextures::getCharTexture(char c) {

	if (isdigit(c)) {
		return charTextures[c - '0'];
	}

	if (islower(c)) {
		c = toupper(c);
	}

	if (c >= 'A' && c <= 'Z') {
		return charTextures[10 + c - 'A'];
	}

	switch (c) {
	case ' ':
		return charTextures[38];
	case '<':
		return charTextures[39];
	case '-':
		return charTextures[40];
	case '>':
		return charTextures[41];
	case '%':
		return charTextures[42];
	case '=':
		return charTextures[43];
	case '/':
		return charTextures[44];
	case ';':
		return charTextures[45];
	case '.':
		return charTextures[46];
	case '+':
		return charTextures[47];
	case '[':
			return charTextures[48];
	case ']':
			return charTextures[49];
	case '?':
			return charTextures[50];
	case '(':
			return charTextures[51];
	case ')':
			return charTextures[52];
	default:
		ostringstream buf;
		buf << "No character texture for character \"" << c << "\"";
		throw runtime_error (buf.str().c_str());
	}

}
