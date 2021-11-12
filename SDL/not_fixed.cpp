#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <thread>
#include <mutex>
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024
//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
SDL_MouseButtonEvent *button;

enum pixel_state
{
    empty,
    gas,
    fluid,
    sand,
    solid,
    fire,
    burning,
    burnt,
    fixed_pos
};
struct position
{
    pixel_state state_now;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    uint16_t temperature;
};
// why must I make this
struct cord_2d
{
    int x_pos;
    int y_pos;
};

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void scr_dump();
void redraw_and_render();
void excecution_finished();
// void sand_sim();

// Thread functions
void polling_thread();

// The ratios
const uint_fast8_t actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;

int main()
{
    std::cout << "MY BAGUETTES ARE ON FIRE" << std::endl;
    // telling it how to scale
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Printing the time
    std::cout << "Time = " << current_time << "\n";
    // setting random seed
    srand(current_time);
    // rise my glorious creation*
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
    SDL_RenderClear(renderer);

    // Now we need to get the ratio

    // PAINT IT BLACK
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    for (int x_pos = 0; x_pos < 0; x_pos++)
    {
        for (int y_pos = 0; y_pos < 0; y_pos++)
        {
            pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].temperature = 0;
            pixels[x_pos][y_pos].state_now = empty;
        }
    }

    // idk why I would keep it here but eh
    // bool quit = false;
    // int i = 0;
    // while (!quit)
    // {

    //     if (event.type == SDL_QUIT)
    //     {
    //         quit = true;
    //         break;
    //         std::cout << "KILL IT NOW " << std::endl;
    //     }
    // }

    // Joining the threads so I dont have to put down a revolution
    SDL_RenderPresent(renderer);

    // std::thread render_thread_pointer(rendering_thread, &renderer);
    std::thread polling_thread_pointer(polling_thread);
    bool quit = false;
    int i = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            //User requests quit
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        i++;
        std::cout << "Itteration rendering" << i << std::endl;
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {

            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
        SDL_RenderPresent(renderer);
    }
    std::cout << "So the threads have started" << std::endl;

    // render_thread_pointer.join();
    polling_thread_pointer.join();

    // So this is the reason it was being dumped LOL
    // scr_dump();
    excecution_finished();
}

void polling_thread()
{
     int mouse_x = 0;
     int mouse_y = 0;

    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            //User requests quit
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            /*
            For some god foresaken reason this should work,
            but like I dont think it will work
            but at this point
            who even cares
            */
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_GetMouseState(&mouse_x, &mouse_y);
                std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
                SDL_RenderDrawPoint(renderer, mouse_x / actual_2_logic_ratio, mouse_y / actual_2_logic_ratio);
                SDL_RenderPresent(renderer);
                pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
                pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
                pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fixed_pos;
                SDL_RenderPresent(renderer);
                break;
            case SDL_BUTTON_RIGHT:
                // set draw colour
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                // Get mouse position
                SDL_GetMouseState(&mouse_x, &mouse_y);
                //Output location
                std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
                // Get mouse position, convert to logical position, then make like a block around it which is 8x8 to make I think white
                for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
                {
                    for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
                    {
                        // Drawing , outputing position, draw, and seting new state
                        SDL_RenderDrawPoint(renderer, x_pos, y_pos);
                        std::cout << "(" << x_pos << "," << y_pos << ")" << std::endl;
                        pixels[x_pos][y_pos].state_now = solid;
                        pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].b = 255;
                    }
                    SDL_RenderPresent(renderer);
                }

                break;
            }
        }
    }
    std::cout << "Polling thread being killed" << std::endl;
}

// Debug functions
void scr_dump()
{
    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            std::cout << pixels[x_pos][y_pos].state_now;
        }
        std::cout << std::endl;
    }
}
void redraw_and_render()
{
    for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
    {
        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);

            if (pixels[x_pos][y_pos].state_now == solid)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            }
        }
    }
}
void excecution_finished(void)
{
    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
