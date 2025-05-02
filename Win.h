#include "Scene.h"

class Win : public Scene {
public:
    //int ENEMY_COUNT = 1;

    ~Win();

    void initialise(int player_heaelth) override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
