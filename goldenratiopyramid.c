#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

int save_surface_to_jpeg(SDL_Surface *surface, const char *filename) {
#ifdef IMG_SAVEJPG_AVAILABLE
    if (IMG_SaveJPG(surface, filename, 100) == 0) {
        return 0; // Success
    } else {
        fprintf(stderr, "Error saving JPEG: %s\n", IMG_GetError());
        return 1; // Failure
    }
#else
    fprintf(stderr, "JPEG saving not supported. Compile with SDL2_image JPEG support.\n");
    return 1;
#endif
}

// Function to draw a circle (Bresenham's algorithm)
void draw_circle(SDL_Renderer *renderer, int center_x, int center_y, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (y >= x) {
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        x++;
    }
}
void drawSquareTangentToCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int squareSize = radius * 2;  // The square's side length is the circle's diameter
    int squareX = centerX - radius;
    int squareY = centerY - radius;

    SDL_Rect square = {
        squareX,
        squareY,
        squareSize,
        squareSize
    };

    SDL_RenderDrawRect(renderer, &square);
}
int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_JPG))) {
        fprintf(stderr, "SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Triangles", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    const double phi = (1.0 + sqrt(5.0)) / 2.0;
    const double sqrt_phi = sqrt(phi);

    const double base = 200;
    const double side = base / 2 * phi;
    const double height = base / 2 * sqrt_phi;

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

        SDL_Point red_points[3] = {
            {WIDTH / 2, HEIGHT / 2 + height / 2},              // C
            {WIDTH / 2 - base / 2, HEIGHT / 2 + height / 2},   // A
            {WIDTH / 2, HEIGHT / 2 - height / 2}              // B
        };
        SDL_RenderDrawLines(renderer, red_points, 3);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_Point blue_points[3] = {
            {WIDTH / 2, HEIGHT / 2 - height / 2},
            {WIDTH / 2 + base / 2, HEIGHT / 2 + height / 2},
            {WIDTH / 2, HEIGHT / 2 + height / 2}
        };
        SDL_RenderDrawLines(renderer, blue_points, 3);
        // Draw Green Circle
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Green
        int circle_x = WIDTH / 2; // Center X (same as triangles' bottom center)
        int circle_y = HEIGHT / 2 + height / 2; // Center Y (bottom of triangles)
        draw_circle(renderer, circle_x, circle_y, (int)(base/2)); // Cast side to int
        drawSquareTangentToCircle(renderer, circle_x, circle_y, base/2);

        // Draw Gold Circle
        SDL_SetRenderDrawColor(renderer, 0xff, 0xd7, 0x00, 0xFF); 
        circle_x = WIDTH / 2; // Center X (same as triangles' bottom center)
        circle_y = HEIGHT / 2 + height / 2; // Center Y (bottom of triangles)
        draw_circle(renderer, circle_x, circle_y, (int)(height)); // Cast side to int


        // Draw Circle
        SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF); 
        circle_x = WIDTH / 2; // Center X (same as triangles' bottom center)
        circle_y = HEIGHT / 2 - height / 2; // Center Y (bottom of triangles)
        draw_circle(renderer, circle_x, circle_y, (int)(height-base/2)); // Cast side to int

        // Draw Square (Corrected)
        SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);
        int square_size = (int)(height-base/2);
        drawSquareTangentToCircle(renderer, circle_x, circle_y, square_size);
        SDL_RenderPresent(renderer);
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_S]) {
            Uint32 window_format = SDL_GetWindowPixelFormat(window);
            SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, WIDTH, HEIGHT, 32, window_format);
            if (surface == NULL) {
                fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat Error: %s\n", SDL_GetError());
                return 1;
            }
            SDL_RenderReadPixels(renderer, NULL, window_format, surface->pixels, surface->pitch);

	    if (SDL_SaveBMP(surface, "triangles.bmp") ==0){
            //if (save_surface_to_jpeg(surface, "triangles.jpg") == 0) {
                printf("Image saved to triangles.jpg\n");
            }

            SDL_FreeSurface(surface);
        }
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
