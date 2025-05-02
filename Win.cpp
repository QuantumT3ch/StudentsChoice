#include "Win.h"
#include "Utility.h"

#define LEVEL_WIDTH 12
#define LEVEL_HEIGHT 10

constexpr char SPRITESHEET_FILEPATH[] = "assets/PlayerSpritesheet.png",
FONT_FILEPATH[] = "assets/font1.png",
SWORD_FILEPATH[] = "assets/sprites/Sword.png",
HELMET_FILEPATH[] = "assets/sprites/HelmetPlayer.png";


unsigned int WIN_DATA[] =
{
    101,101,101,101,101,101,101,101,101,101,101,101,
    101,101,101,101,101,101,101,101,101,101,101,101,
    101,15, 0,  16, 2,  2,  2,  2,  17, 0,  13, 101,
    101,15, 0,  57, 29, 40, 40, 29, 59, 0,  13, 101,
    101,16, 2,  2,  2,  2,  2,  2,  2,  2,  17, 101,
    76, 57, 28, 40, 40, 22, 23, 40, 40, 28, 59, 77,
    51, 50, 51, 50, 51, 42, 42, 50, 107, 50, 51, 50,
    48, 64, 48, 65, 48, 42, 98, 48, 119, 106,107, 48,
    48, 66, 48, 66, 48, 48, 48, 48, 131, 48, 119, 48,
    48, 48, 48, 48, 48, 48, 42, 48, 48, 48, 131, 48,

};

Win::~Win()
{

    delete    m_game_state.player;
    delete    m_game_state.map;
    delete    m_game_state.weapon;
}

void Win::initialise(int player_health)
{
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/tilemap_packed.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, WIN_DATA, map_texture_id, 1.0f, 12, 11);



}

void Win::update(float delta_time)
{

}

void Win::render(ShaderProgram* program)
{
    
     
     
    program->set_light_position_matrix(glm::vec3(6.0f, -7.0f, 0.0f));

    m_game_state.map->render(program);
    program->turn_lights_on(0);
    Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), "You Escaped!", 0.75f, 0.001f,
        glm::vec3(2.2f, -6.0f, 0.0f));

    program->turn_lights_on(1);

}
