#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "string"

// Class containing the texture
class LTexture
{
public:
    // Initialization
    LTexture();

    ~LTexture();

    // Loading a file into a texture
    bool loadFromFile(std::string path, SDL_Renderer *gRenderer);

#if defined(SDL_TTF_MAJOR_VERSION)
    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font *gFont, SDL_Renderer *gRenderer);
#endif

    // Removing texture
    void free();

    // Flashing texture
    void flashing();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    void setBlendMode(SDL_BlendMode blending);

    void setAlphaMod();

    Uint8 getAlpha() { return alpha; }
    void setNewAlpha(Uint8 ALPHA);

    // Render texture to screen
    void render(int x, int y, SDL_Renderer *gRenderer, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }

private:
    // The texture itself
    SDL_Texture *mTexture;

    int mWidth;
    int mHeight;
    Uint8 alpha;
};