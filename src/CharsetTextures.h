/*
 * CharsetTextures.h
 *
 *  Created on: Apr 10, 2017
 *      Author: shahada
 */

#ifndef CHARSETTEXTURES_H_
#define CHARSETTEXTURES_H_

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
	SDL_Texture * charTextures [53];

	void generateTextureForChar (std::string c, int index, TTF_Font * font, SDL_Renderer * renderer, SDL_Color textColor, SDL_Color backgroundColor);
public:
	CharsetTextures(std::string fontPath, int fontHeight, SDL_Renderer * renderer);
	virtual ~CharsetTextures();

	SDL_Texture * getCharTexture (char c);

	TTF_Font * getFont (std::string fontPath, int MaxHeight, SDL_Renderer * renderer);
};

#endif /* CHARSETTEXTURES_H_ */
