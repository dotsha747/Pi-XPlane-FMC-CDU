/*
 * CharsetTextures.h
 *
 *  Created on: Apr 10, 2017
 *      Author: shahada
 */

#ifndef PIXPLANEFMCCDU_SRC_CHARSETTEXTURES_H_
#define PIXPLANEFMCCDU_SRC_CHARSETTEXTURES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/** @brief character font texture cache
 *
 * 	Builds and caches textures for the following characters:
 * 		0-9: 0-9
 * 		11-37: A-Z
 * 		38 : [space]
 * 		39 : <
 * 		40 : -
 * 		41 : >
 * 		42 : %
 * 		43 : ° (=)
 * 		44 : /
 * 		45 : ;
 * 		46 : .
 * 		47 : +
 * 		48 : [
 * 		49 : ]
 * 		50 : ?
 * 		51 : (
 * 		52 : )
 *
 *	Above indexes are internal, use getCharTexture() to obtain textures.
 *
 *	Assumes TTF_Init has already been called.
 */


class CharsetTextures {
protected:

	int cellWidth;
	int cellHeight;
	SDL_Texture * allCharTexture;

	TTF_Font * getFont (std::string fontPath, int colWidth, int MaxHeight, SDL_Renderer * renderer);
	void generateTextureForChar (std::string c, int index, int rowHeight, TTF_Font * font, SDL_Renderer * renderer, SDL_Color textColor, SDL_Color backgroundColor, SDL_Surface allCharSurface);

public:
	CharsetTextures(std::string fontPath, SDL_Window * window, SDL_Renderer * renderer, int colWidth, int fontHeight, SDL_Color fgcolor, SDL_Color bgcolor);
	virtual ~CharsetTextures();
	void renderCharAt (SDL_Renderer * renderer, char c, int x, int y);

};

#endif /* PIXPLANEFMCCDU_SRC_CHARSETTEXTURES_H_ */
