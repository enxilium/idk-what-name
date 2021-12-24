// Polling for user input
// This is gonna be painful
// #include "base_data.hpp"
#include <SDL2/SDL.h>
bool poll_usr_input(bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], SDL_Event *event, bool quit, int actual_2_logic_ratio)
{
    bool quit_now = false;
    int mouse_x = 0;
    int mouse_y = 0;
    while(SDL_PollEvent(event) != 0)
    {
        // idk why this doesn't
        // if (SDL_PollEvent(event) && event->type == SDL_QUIT)
        // {
        //     quit_now = true;
        //     std::cout << "SHOULD NOW STOP" << std::endl;
        // }

        // if ((event->motion.state & SDL_BUTTON_LMASK) == SDL_BUTTON_LEFT)
        // {
        //     SDL_GetMouseState(&mouse_x, &mouse_y);
        //     std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 148;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 204;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 222;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = solid;
        //     changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
        //     std::cout << "Left" << std::endl;
        // }

        // if ((event->motion.state & SDL_BUTTON_RMASK) == SDL_BUTTON_RIGHT)
        // {
        //     SDL_GetMouseState(&mouse_x, &mouse_y);
        //     std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 153;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 0;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
        //     usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = solid;
        //     changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
        //     std::cout << "Right" << std::endl;
        // }

        switch(event->motion.state)
        {
            case SDL_QUIT:
                std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 148;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 204;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 222;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = solid;
                changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
                std::cout << "Left" << std::endl;
                break;
            case SDL_BUTTON_RIGHT:
                SDL_GetMouseState(&mouse_x, &mouse_y);
                std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = 153;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = 0;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
                usr_input[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = solid;
                changed[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio] = true;
                std::cout << "Right" << std::endl;
                break;
            default:
                break;
        }
    }

    return quit_now;
}

// Gonna throw this part onto the backburner for now whilst I figure out what the heck is going on
void mix_new_version_usr_input(bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{
    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
    {
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            if(changed[x_pos][y_pos] == true)
            {
                // Doesn't work dumbass
                // new_version[x_pos][y_pos] = usr_input[x_pos][y_pos];
                new_version[x_pos][y_pos].r             = usr_input[x_pos][y_pos].r          ;
                new_version[x_pos][y_pos].g             = usr_input[x_pos][y_pos].g          ;
                new_version[x_pos][y_pos].b             = usr_input[x_pos][y_pos].b          ;
                new_version[x_pos][y_pos].a             = usr_input[x_pos][y_pos].a          ;
                new_version[x_pos][y_pos].state_now     = usr_input[x_pos][y_pos].state_now  ;
                new_version[x_pos][y_pos].pressure      = usr_input[x_pos][y_pos].pressure   ;
                new_version[x_pos][y_pos].temperature   = usr_input[x_pos][y_pos].temperature;

                usr_input[x_pos][y_pos].r = usr_input[x_pos][y_pos].g = usr_input[x_pos][y_pos].b = usr_input[x_pos][y_pos].a = usr_input[x_pos][y_pos].temperature = usr_input[x_pos][y_pos].pressure = 0;
                usr_input[x_pos][y_pos].state_now = empty;
                changed[x_pos][y_pos] = false;
            }
            
        }
    }
}