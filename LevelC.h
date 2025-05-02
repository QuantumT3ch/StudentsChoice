#include "Scene.h"

class LevelC : public Scene {
public:
    int ENEMY_COUNT = 9;

    ~LevelC();

    void initialise(int player_health) override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};
