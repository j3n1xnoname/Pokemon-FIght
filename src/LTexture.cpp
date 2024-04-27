#include "LTexture.hpp"

// Initialization
LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
    alpha = 255;
}

LTexture::~LTexture()
{
    free();
}

// Removing texture
void LTexture::free()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

// Loading a file into a texture
bool LTexture::loadFromFile(std::string path, SDL_Renderer *gRenderer)
{
    bool success = true;
    free();
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Failed to load image! IMG_Error: %s\n", IMG_GetError());
        success = false;
    }

    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL)
    {
        printf("Erorr creating texture from surface! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
    loadedSurface = NULL;

    mTexture = newTexture;

    return success;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlphaMod()
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setNewAlpha(Uint8 ALPHA)
{
    alpha = ALPHA;
    setAlphaMod();
}

void LTexture::render(int x, int y, SDL_Renderer *gRenderer, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if (clip != NULL)
    {
        renderQuad.h = clip->h;
        renderQuad.w = clip->w;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font *gFont, SDL_Renderer *gRenderer)
{
    free();

    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);

    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (mTexture == NULL)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }

    mWidth = textSurface->w;
    mHeight = textSurface->h;

    SDL_FreeSurface(textSurface);

    return mTexture != NULL;
}
#endif

void LTexture::flashing()
{
    alpha -= 3;
    if (alpha < 0)
    {
        alpha = 255;
    }
    setAlphaMod();
}