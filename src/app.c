#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Include SDL_ttf header for font rendering
#include <emscripten/emscripten.h> // Include Emscripten header

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
SDL_Texture* textTexture = NULL;
SDL_Rect textRect;

void main_loop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop(); // Stop the main loop when window is closed
            return;
        }
    }

    // Clear the screen with blue
    SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
    SDL_RenderClear(renderer);

    // Render the text
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Present the renderer
    SDL_RenderPresent(renderer);
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a window
    window = SDL_CreateWindow(
        "SDL in Browser",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        320,
        320,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load a font
    font = TTF_OpenFont("src/assets/font.ttf", 24); // Replace with your font path
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a surface with the text
    SDL_Color textColor = {0, 0, 0, 255}; // White text color
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Hi Dokyung :) This is written in C", textColor);
    if (!textSurface) {
        printf("TTF_RenderText_Blended Error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a texture from the surface
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect = (SDL_Rect){40, 140, textSurface->w, textSurface->h}; // Position and size of text
    SDL_FreeSurface(textSurface);

    // Set up the main loop for Emscripten
    emscripten_set_main_loop(main_loop, 0, 1);

    // Cleanup is handled when the main loop exits
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
