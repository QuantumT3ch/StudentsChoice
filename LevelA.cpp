#include "LevelA.h"
#include "Utility.h"
#include "vector"

#define LEVEL_WIDTH 22
#define LEVEL_HEIGHT 10
//All textures are taken from here
// Source: https://kenney.nl/assets/tiny-dungeon

constexpr char HELMET_FILEPATH[] = "assets/sprites/HelmetPlayer.png",
ARMOR_FILEPATH[] = "assets/sprites/ArmoredPlayer.png",
CLOTH_FILEPATH[] = "assets/sprites/ClothedPlayer.png",
GRAVE_FILEPATH[] = "assets/sprites/Grave.png",
SWORD_FILEPATH[] = "assets/sprites/Sword.png",
SLASH_FILEPATH[] = "assets/sprites/Slash.png",
ENEMY_FILEPATH[] = "assets/sprites/Ghoul.png";



unsigned int LEVELA_DATA[] =
{ 
   1,  2,  2,  2,  2,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   13, 40, 46, 47, 40, 15, 1,  2,  2,  2,  2,  2,  2,  3,  0,  0,  0,  0,  0,  0,  0,  0, 
   13, 51, 50, 51, 50, 15, 13, 40, 28, 40, 40, 28, 40, 15, 1,  2,  2,  2,  3,  0,  0,  0,
   13, 48, 48, 48, 48, 15, 13, 50, 51, 50, 51, 50, 51, 15, 13, 64, 48, 65, 15, 0,  0,  0,
   13, 48, 49, 48, 48, 16, 17, 48, 48, 49, 48, 48, 48, 15, 13, 66, 48, 66, 15, 0,  0,  0,
   13, 48, 48, 48, 48, 57, 59, 48, 48, 4,  5,  48, 48, 16, 17, 48, 48, 48, 16, 2,  2,  3,
   13, 49, 48, 48, 48, 50, 51, 48, 48, 15, 13, 49, 48, 57, 59, 36, 37, 38, 57, 46, 47, 15,
   13, 63, 48, 49, 48, 48, 49, 48, 48, 15, 13, 48, 48, 50, 51, 48, 49, 48, 50, 51, 50, 15,
   13, 75, 63, 48, 48, 48, 48, 49, 48, 15, 13, 48, 49, 48, 48, 48, 48, 48, 48, 48, 48, 15,
   25, 26, 26, 26, 26, 26, 26, 26, 26, 27, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27

};
unsigned int LEVELA_DOOR[] =
{
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 22, 23, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
   56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,

};

LevelA::~LevelA()
{
    for (Entity* enemy : m_game_state.enemies) {
        delete    enemy;
    }
    m_game_state.enemies.clear();
    delete    m_game_state.player;
    delete    m_game_state.map;

    Mix_FreeChunk(m_game_state.attack_sfx);
    Mix_FreeChunk(m_game_state.door_unlocked);
    Mix_FreeChunk(m_game_state.level_clear_sfx);
    Mix_FreeChunk(m_game_state.death_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise(int player_health)
{
    m_game_state.next_scene_id = -1;
    m_game_state.enemies_alive = ENEMY_COUNT;
    
    GLuint map_texture_id = Utility::load_texture("assets/tilemap_packed.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 12, 11);
    m_game_state.doors = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DOOR, map_texture_id, 1.0f, 12, 11);
    
    m_game_state.LEFT_EDGE = 4.5f;
    m_game_state.RIGHT_EDGE = 16.5f;
    m_game_state.BOTTOM_EDGE = -5.75f;
    m_game_state.TOP_EDGE = -3.5f;

    int player_walking_animation[4][4] =
    {
        {  0, 0, 0, 0 },  // for George to move to the left,
        { 1, 1, 1, 1 }, // for George to move to the right,
        { 1, 1, 1, 1 }, // for George to move upwards,
        { 1, 1, 1, 1 }   // for George to move downwards
    };

    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    GLuint player_texture_id;

    if (player_health == 2) {
         player_texture_id = Utility::load_texture(ARMOR_FILEPATH);
    }
    else if (player_health == 1) {
         player_texture_id = Utility::load_texture(CLOTH_FILEPATH);
    }
    else {
        player_texture_id = Utility::load_texture(HELMET_FILEPATH);
    }
    
     
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        0.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        2,                         // animation frame amount
        0,                         // current animation index
        2,                         // animation column amount
        1,                         // animation row amount
        1.0f,                      // width
        1.0f,                       // height
        PLAYER
    );

    m_game_state.player->set_health(player_health);
     
    m_game_state.player->set_position(glm::vec3(2.0f, -2.0f, 0.0f));

    m_game_state.sword_texture_id = Utility::load_texture(SWORD_FILEPATH);
    m_game_state.slash_texture_id = Utility::load_texture(SLASH_FILEPATH);
    m_game_state.weapon = new Entity(m_game_state.sword_texture_id, 0.0f, 1.0f, 1.0f, WEAPON);
    m_game_state.player->set_weapon(m_game_state.weapon); // Simpler constructor

    
    m_game_state.weapon->follow_player(glm::vec3(2.0f, 0.0f, 0.0f), m_game_state.player);
    m_game_state.weapon->set_scale(glm::vec3(0.75f, 0.75f, 0.0f));


    
    
    /**
    Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    int enemy_walking_animation[4][4] =
    {
        { 0, 0, 0, 0 }, 
        { 0, 0, 0, 0 }, 
        { 0, 0, 0, 0 }, 
        { 0, 0, 0, 0 }  
    };

    m_game_state.enemies.clear();

    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        
        m_game_state.enemies.push_back(new Entity(
            enemy_texture_id,         // texture id
            1.0f,                      // speed
            acceleration,              // acceleration
            0.0f,                      // jumping power
            enemy_walking_animation,  // animation index sets
            0.0f,                      // animation time
            1,                         // animation frame amount
            0,                         // current animation index
            1,                         // animation column amount
            1,                         // animation row amount
            1.0f,                      // width
            1.0f,                       // height
            ENEMY
        ));
        m_game_state.enemies[i]->set_ai_type(GUARD);
        m_game_state.enemies[i]->set_ai_state(IDLE);
        m_game_state.enemies[i]->set_health(2);
    }
    

    m_game_state.enemies[0]->set_position(glm::vec3(11.0f, -3.0f, 0.0f));
    m_game_state.enemies[0]->set_movement(glm::vec3(0.0f));

    m_game_state.enemies[1]->set_position(glm::vec3(15.0f, -5.0f, 0.0f));
    m_game_state.enemies[1]->set_movement(glm::vec3(0.0f));

    m_game_state.enemies[2]->set_position(glm::vec3(17.0f, -5.0f, 0.0f));
    m_game_state.enemies[2]->set_movement(glm::vec3(0.0f));
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    
    // Source: https://incompetech.com/music/royalty-free/music.html
    m_game_state.bgm = Mix_LoadMUS("assets/audio/Chill.mp3");
    
    // Source: https://opengameart.org/content/slime-jump-effect
    m_game_state.attack_sfx = Mix_LoadWAV("assets/audio/attack.wav");
    // Source: https://opengameart.org/content/damage-taken
    m_game_state.damage_sfx = Mix_LoadWAV("assets/audio/damage_taken.wav");
    // Source: https://opengameart.org/content/8-bit-sound-effects-library
    m_game_state.death_sfx = Mix_LoadWAV("assets/audio/player_death.wav");

    m_game_state.level_clear_sfx = Mix_LoadWAV("assets/audio/door_open.wav");

    m_game_state.door_unlocked = Mix_LoadWAV("assets/audio/Key Jiggle.wav");
    m_game_state.death_flag = false;
    Mix_VolumeChunk(
        m_game_state.attack_sfx,
        MIX_MAX_VOLUME / 8  
    );
    Mix_VolumeChunk(
        m_game_state.death_sfx,  
        MIX_MAX_VOLUME / 2 
    );
    Mix_VolumeChunk(
        m_game_state.level_clear_sfx,   
        MIX_MAX_VOLUME / 8  
    );
}


void LevelA::update(float delta_time)
{
    if (m_game_state.player->was_damaged()) {


        Mix_PlayChannel(-1, m_game_state.damage_sfx, 0);

        if (m_game_state.player->get_health() == 2) {
            m_game_state.player->set_texture_id(Utility::load_texture(ARMOR_FILEPATH));
        }
        else if (m_game_state.player->get_health() == 1) {
            m_game_state.player->set_texture_id(Utility::load_texture(CLOTH_FILEPATH));
        }
        //m_game_state.player->set_position(glm::vec3(2.0f, -2.0f, 0.0f));
    }

    if (m_game_state.player->is_attacking()) {

        m_game_state.attack_timer = 1000 * delta_time;
        m_game_state.weapon->set_texture_id(m_game_state.slash_texture_id);

        if (m_game_state.player->is_attacking_right()) {
            m_game_state.weapon_position = glm::vec3(0.5f, 0.0f, 0.0f);
        }
        else if (m_game_state.player->is_attacking_left()) {
            m_game_state.weapon_position = glm::vec3(-0.5f, 0.0f, 0.0f);
        }
        else if (m_game_state.player->is_attacking_up()) {
            m_game_state.weapon_position = glm::vec3(0.0f, 0.5f, 0.0f);
        }
        else {
            m_game_state.weapon_position = glm::vec3(0.0f, -0.5f, 0.0f);
        }
        m_game_state.weapon->follow_player(m_game_state.weapon_position, m_game_state.player);
    }

    if (m_game_state.attack_timer > 0) {
        m_game_state.weapon->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, NULL);
        m_game_state.attack_timer -= delta_time;
    }
    else {
        m_game_state.weapon->set_texture_id(m_game_state.sword_texture_id);
        m_game_state.weapon->update(delta_time, m_game_state.player, m_game_state.enemies, 0, NULL);
        m_game_state.weapon_position = glm::vec3(0.5f, 0.0f, 0.0f);
    }

    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    if (m_game_state.player->is_dead()) {

        if (!m_game_state.death_flag) {
            m_game_state.death_flag = true;
            
            m_game_state.player->set_animation_cols(1);
            m_game_state.weapon->deactivate();
            m_game_state.player->set_texture_id(Utility::load_texture(GRAVE_FILEPATH));

            Mix_PlayChannel(-1, m_game_state.death_sfx, 0);
        }

    }
    m_game_state.weapon->follow_player(m_game_state.weapon_position, m_game_state.player);

    m_game_state.enemies_alive = ENEMY_COUNT;

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        if (m_game_state.enemies[i]->is_active()) {
            m_game_state.enemies[i]->update(delta_time, m_game_state.player, m_game_state.empty_vector, 0, m_game_state.map);
        }
        else {
            m_game_state.enemies_alive -= 1;
        }

    }

    if (!m_game_state.door_unlocked_flag && m_game_state.enemies_alive == 0) {
        m_game_state.door_unlocked_flag = true;
        Mix_PlayChannel(-1, m_game_state.door_unlocked, 0);
    }


    if (m_game_state.door_unlocked_flag &&
        m_game_state.player->get_position().x > 18.8f
        && m_game_state.player->get_position().y > -7.2f) {
        Mix_PlayChannel(-1, m_game_state.level_clear_sfx, 0);
        m_game_state.next_scene_id = 2;
    }

    
}

void LevelA::render(ShaderProgram *program)
{
    program->set_light_position_matrix(m_game_state.player->get_position());
    program->player_took_damage(0);
    m_game_state.map->render(program);
    if (m_game_state.door_unlocked_flag) {
        m_game_state.doors->render(program);
    }
    
    if (m_game_state.player->get_i_frames() <= 0) {
        program->player_took_damage(0);
    }
    else {
        program->player_took_damage(1);
    }
    
    m_game_state.player->render(program);

    program->player_took_damage(0);
    m_game_state.weapon->render(program);


    for (int i = 0; i < ENEMY_COUNT; i++)
    {
   
        if (m_game_state.enemies[i]->get_i_frames() <= 0) {
            program->player_took_damage(0);
        }
        else {
            program->player_took_damage(1);
        }
        m_game_state.enemies[i]->render(program);
        program->player_took_damage(0);
    }
    
    
}
