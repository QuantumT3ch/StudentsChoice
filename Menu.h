#include "Scene.h"

class Menu : public Scene {
public:
    //int ENEMY_COUNT = 1;

    ~Menu();

    void initialise(int player_heaelth) override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
#pragma once
