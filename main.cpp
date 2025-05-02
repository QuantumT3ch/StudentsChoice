/**
* Author: Connor Chavez
* Assignment: Clear the Dungeon!
* Date due: 5/2, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f
#define LEVEL1_BOTTOM_EDGE -4.75f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include"Menu.h"
#include"Win.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "Effects.h"

// ––––– CONSTANTS ––––– //
constexpr int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;
//Level 1menu background
constexpr float BG1_RED = 0.914f,
BG1_BLUE = 0.422f,
BG1_GREEN = 0.645f,
BG1_OPACITY = 1.0f;

constexpr float BG2_RED = 0.629f,
BG2_BLUE = 0.917f,
BG2_GREEN = 0.871f,
BG2_OPACITY = 1.0f;

constexpr float BG3_RED = 0.422f,
BG3_BLUE = 0.5f,
BG3_GREEN = 0.383f,
BG3_OPACITY = 1.0f;


constexpr int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_LIT_SHADER_PATH[] = "shaders/vertex_lit.glsl",
F_LIT_SHADER_PATH[] = "shaders/fragment_lit.glsl",
//V_SHADER_PATH[] = "shaders/vertex.glsl",
//F_SHADER_PATH[] = "shaders/fragment.glsl",
FONT_FILEPATH[] = "assets/font1.png",
//All textures are taken from here
// Source: https://kenney.nl/assets/pico-8-platformer
FULL_HEART_FILEPATH[] = "assets/sprites/FullShield.png",
EMPTY_HEART_FILEPATH[] = "assets/sprites/EmptyShield.png";

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

enum AppStatus { RUNNING, TERMINATED };

// ––––– GLOBAL VARIABLES ––––– //
Scene* g_current_scene;
Scene* g_old_scene;
Menu* g_menu;
LevelA* g_levelA;
LevelB* g_levelB;
LevelC* g_levelC;
Win* g_win;


Effects* g_effects;
Scene* g_levels[5];

int next_scene;

float camera_x,
camera_y,
camera_left_constraint,
camera_right_constraint,
camera_bottom_constraint,
camera_top_constraint,
enemy_counter_x,
enemy_counter_y;

int old_lives,
current_lives,
total_lives = 3;



GLuint full_heart_texture_id,
empty_heart_texture_id,
lives_texture_id;

glm::vec3 g_player_position;

Entity* lives;

GLuint font_texture_id;

SDL_Window* g_display_window;


ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

bool g_is_colliding_bottom = false;

AppStatus g_app_status = RUNNING;

void swtich_to_scene(Scene* scene);
void initialise();
void process_input();
void update();
void render();
void shutdown();

// ––––– GENERAL FUNCTIONS ––––– //
void switch_to_scene(Scene* scene)
{

    g_old_scene = g_current_scene;
    g_current_scene = scene;
    g_current_scene->initialise(total_lives); // DON'T FORGET THIS STEP!

    if (g_old_scene != scene) {
        if (scene == g_levelA ) {
            Mix_PlayMusic(scene->get_state().bgm, -1);
            Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
        }

        if (scene == g_win) {
            g_view_matrix = glm::mat4(1.0f);
            //g_projection_matrix = glm::ortho(-5.0f, 7.0f, -6.0f, 2.0f, -1.0f, 1.0f);
            g_projection_matrix = glm::ortho(-0.5f, 11.5f, -9.5f, -1.5f, -1.0f, 1.0f);
            g_shader_program.set_projection_matrix(g_projection_matrix);
        }
        
    }
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Clear The Dungeon!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_LIT_SHADER_PATH, F_LIT_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(0.0f, 10.0f, -6.75f, 0.0f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    //g_shader_program.player_took_damage(0);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(0, 0, 0, 1);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    g_menu = new Menu();
    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_levelC = new LevelC();
    g_win = new Win();
    g_levels[0] = g_menu;
    g_levels[1] = g_levelA;
    g_levels[2] = g_levelB;
    g_levels[3] = g_levelC;
    g_levels[4] = g_win;

    // Start at Menu
    switch_to_scene(g_levels[0]);

    g_effects = new Effects(g_projection_matrix, g_view_matrix);
    


    //Mix_PlayChannel(-1, g_current_scene->get_state().jump_sfx, 0);
    //Mix_PlayMusic(g_levelA->get_state().bgm, -1);
    //Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    Mix_Music* bgm = Mix_LoadMUS("assets/audio/Chill.mp3");
    Mix_PlayMusic(bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    font_texture_id = Utility::load_texture(FONT_FILEPATH);

    full_heart_texture_id = Utility::load_texture(FULL_HEART_FILEPATH);
    empty_heart_texture_id = Utility::load_texture(EMPTY_HEART_FILEPATH);



    lives = new Entity[3];


    for (int i = 0; i < 3; i++) {

        lives[i] = Entity(
            full_heart_texture_id,
            0.0f,
            1.0f,
            1.0f,
            ITEM);


    }
    




}

int attack_delay = 0;

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    if (g_current_scene != g_menu && g_current_scene != g_win) {
        g_current_scene->get_state().player->set_movement(glm::vec3(0.0f));
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_app_status = TERMINATED;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_app_status = TERMINATED;
                break;

            case SDLK_RETURN:
                if (g_current_scene == g_menu) {
                    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
                    g_shader_program.set_projection_matrix(g_projection_matrix);
                    switch_to_scene(g_levelA);
                }

                break;
            ;

            
            case SDLK_RIGHT:
                if (attack_delay == 0) {
                    attack_delay = 1500 * FIXED_TIMESTEP;
                    g_current_scene->get_state().player->attack_right();
                    Mix_PlayChannel(-1, g_current_scene->get_state().attack_sfx, 0);
                }
                
                break;
            case SDLK_LEFT:

                if (attack_delay == 0) {
                    attack_delay = 1500 * FIXED_TIMESTEP;
                    g_current_scene->get_state().player->attack_left();
                    Mix_PlayChannel(-1, g_current_scene->get_state().attack_sfx, 0);
                }
                break;
            case SDLK_UP:

                if (attack_delay == 0) {
                    attack_delay = 1500 * FIXED_TIMESTEP;
                    g_current_scene->get_state().player->attack_up();
                    Mix_PlayChannel(-1, g_current_scene->get_state().attack_sfx, 0);
                }
                break;
            case SDLK_DOWN:

                if (attack_delay == 0) {
                    attack_delay = 1500 * FIXED_TIMESTEP;
                    g_current_scene->get_state().player->attack_down();
                    Mix_PlayChannel(-1, g_current_scene->get_state().attack_sfx, 0);
                }
                break;
            case SDLK_1:
                switch_to_scene(g_levelA);

                break;
            case SDLK_2:
                switch_to_scene(g_levelB);

                break;
            case SDLK_3:
                switch_to_scene(g_levelC);

                break;
            case SDLK_4:
                switch_to_scene(g_win);

                break;
            default:
                break;
            }

        default:
            break;
        }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if (g_current_scene == g_menu || g_current_scene == g_win) return;
    if (g_current_scene->get_state().player->is_dead()) return;
    if (g_current_scene->get_state().next_scene_id == 4) return;

    if (key_state[SDL_SCANCODE_A])        g_current_scene->get_state().player->move_left();
    else if (key_state[SDL_SCANCODE_D])  g_current_scene->get_state().player->move_right();
    else if (key_state[SDL_SCANCODE_S])  g_current_scene->get_state().player->move_down();
    else if (key_state[SDL_SCANCODE_W])  g_current_scene->get_state().player->move_up();

    if (glm::length(g_current_scene->get_state().player->get_movement()) > 1.0f)
        g_current_scene->get_state().player->normalise_movement();

}




void update()
{

    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;



    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }
    next_scene = g_current_scene->get_state().next_scene_id;

    while (delta_time >= FIXED_TIMESTEP && total_lives > 0 
        && g_current_scene!=g_menu 
        && g_current_scene != g_win
        && g_current_scene->get_state().next_scene_id != 4) {
        old_lives = g_current_scene->get_state().player->get_health();
        g_current_scene->update(FIXED_TIMESTEP);
        g_player_position = g_current_scene->get_state().player->get_position();
        if (attack_delay > 0) {
            attack_delay -= FIXED_TIMESTEP;
        }


        

        current_lives = g_current_scene->get_state().player->get_health();


        if (total_lives <= 0) {
            //switch_to_scene(g_levelB);
        }
        else if (current_lives < old_lives) {

            total_lives -= 1;

            lives[total_lives].set_texture_id(empty_heart_texture_id);

            

        }

        g_effects->update(FIXED_TIMESTEP);

        

        g_is_colliding_bottom = g_current_scene->get_state().player->get_collided_bottom();



        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;

    // Prevent the camera from showing anything outside of the "edge" of the level
    g_view_matrix = glm::mat4(1.0f);
    camera_left_constraint = g_current_scene->get_state().LEFT_EDGE;
    camera_right_constraint = g_current_scene->get_state().RIGHT_EDGE;
    camera_bottom_constraint = g_current_scene->get_state().BOTTOM_EDGE;
    camera_top_constraint = g_current_scene->get_state().TOP_EDGE;

    if (g_player_position.x > camera_left_constraint && g_player_position.x < camera_right_constraint) {

        camera_x = -g_player_position.x;
        enemy_counter_x = g_player_position.x-4.5;
    }
    else if (g_player_position.x < camera_left_constraint) {
        camera_x = -camera_left_constraint;
        enemy_counter_x = camera_left_constraint-4.5;
    }
    else {
        camera_x = -camera_right_constraint;
        enemy_counter_x = camera_right_constraint-4.5;
    }

    if (g_player_position.y > camera_bottom_constraint && g_player_position.y < camera_top_constraint) {

        camera_y = -g_player_position.y;
        enemy_counter_y = g_player_position.y+3.5;
    }
    else if (g_player_position.y < camera_bottom_constraint) {
        camera_y = -camera_bottom_constraint;
        enemy_counter_y = camera_bottom_constraint+3.5;
    }
    else {
        camera_y = -camera_top_constraint;
        enemy_counter_y = camera_top_constraint+3.5;
    }
    for (int i = 0; i < 3; i++) {
        lives[i].set_position(glm::vec3(0.25f+i + enemy_counter_x, enemy_counter_y - 0.75f, 0.0f));
        lives[i].update(delta_time, g_current_scene->get_state().player, g_current_scene->get_state().empty_vector, 0, NULL);
    }

    g_view_matrix = glm::translate(g_view_matrix, glm::vec3(camera_x, camera_y, 0));

}
std::string enemy_counter;

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    g_shader_program.turn_lights_on(1);


    glClear(GL_COLOR_BUFFER_BIT);

    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //

    g_current_scene->render(&g_shader_program);

    g_effects->render();

    g_shader_program.turn_lights_on(0);
    if (g_current_scene != g_menu && g_current_scene != g_win) {
        for (int i = 0; i < total_lives; i++) {
            lives[i].render(&g_shader_program);
        }
    }
    
    g_shader_program.turn_lights_on(1);
    if (g_current_scene != g_menu && g_current_scene != g_win) {
        g_shader_program.turn_lights_on(0);
        enemy_counter = "Enemies Left: "+ std::to_string(g_current_scene->get_state().enemies_alive);
        Utility::draw_text(&g_shader_program, font_texture_id, enemy_counter, 0.5f, 0.0f,
            glm::vec3(
                 enemy_counter_x,
                enemy_counter_y,
                0.0f));
        g_shader_program.turn_lights_on(1);
    }

    
    

    next_scene = g_current_scene->get_state().next_scene_id;
    if (total_lives <= 0) {
        g_shader_program.turn_lights_on(0);
        Utility::draw_text(&g_shader_program, font_texture_id, "You Died", 0.5f, 0.001f,
            glm::vec3(
                g_current_scene->get_state().player->get_position().x - 1.5,
                g_current_scene->get_state().player->get_position().y + 1.0,
                0.0f));
        g_shader_program.turn_lights_on(1);
    }

    else if (total_lives > 0 && next_scene == 4) {
        Utility::draw_text(&g_shader_program, font_texture_id, "You Escaped!", 0.5f, 0.001f,
            glm::vec3(
                g_current_scene->get_state().player->get_position().x - 2.5,
                g_current_scene->get_state().player->get_position().y + 1.0,
                0.0f));
    }

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    delete g_levelA;
    delete g_levelB;
    delete g_menu;
    delete g_levelC;
    delete g_effects;
    delete[] lives;
}

// ––––– DRIVER GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();

    while (g_app_status == RUNNING)
    {
        process_input();
        if (g_current_scene != g_menu && g_current_scene != g_win) {
            update();
        }

        next_scene = g_current_scene->get_state().next_scene_id;
        if (next_scene >= 0 && next_scene != 5) {

            switch_to_scene(g_levels[g_current_scene->get_state().next_scene_id]);
        }


        render();
    }

    shutdown();
    return 0;
}