#include "Menu.h"
#include "Utility.h"

#define LEVEL_WIDTH 11
#define LEVEL_HEIGHT 10

constexpr char SPRITESHEET_FILEPATH[] = "assets/PlayerSpritesheet.png",
FONT_FILEPATH[] = "assets/font1.png",
SWORD_FILEPATH[] = "assets/sprites/Sword.png",
HELMET_FILEPATH[] = "assets/sprites/HelmetPlayer.png";

GLuint menu_sword_texture_id;
glm::vec3 player_position;
unsigned int MENU_DATA[] =
{
    101,101,101,101,101,101,101,101,101,101,101,
    101,101,101,101,101,121,101,101,108,101,101,
    101,108,101,101,101,101,101,101,101,101,101,
    101,101,101,101,101,101,101,101,101,101,101,
    101,101,101,101,101,97,101,101,101,101,101,
    101,101,101,101,101,101,101,101,101,101,101,
    101,101,121,101,101,101,101,101,120,101,101,
    101,101,101,101,101,101,101,101,101,101,101,
    101,101,121,101,101,101,101,101,101,101,101,
    101,101,101,101,101,101,101,101,120,101,101,

};

Menu::~Menu()
{
    
    delete    m_game_state.player;
    delete    m_game_state.map;
    delete    m_game_state.weapon;
}

void Menu::initialise(int player_health)
{
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/tilemap_packed.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 12, 11);
    
    
    
}

void Menu::update(float delta_time)
{
    
}

void Menu::render(ShaderProgram* program)
{
    
    
    player_position = glm::vec3(4.5f, -3.0f, 0.0f);
    program->set_light_position_matrix(player_position);

    m_game_state.map->render(program);
    Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), "Press Enter to Start", 0.5f, 0.001f,
        glm::vec3(0.2f, -5.0f, 0.0f));
    Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), "WASD", 0.4f, 0.001f,
        glm::vec3(1.6f, -3.0f, 0.0f));
    Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), "to move", 0.4f, 0.001f,
        glm::vec3(1.0f, -3.4f, 0.0f));
    Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), "Arrows", 0.4f, 0.001f,
        glm::vec3(6.4f, -3.0f, 0.0f));
    Utility::draw_text(program, Utility::load_texture(FONT_FILEPATH), "to attack", 0.4f, 0.001f,
        glm::vec3(6.0f, -3.4f, 0.0f));
    
    
    
    
}
