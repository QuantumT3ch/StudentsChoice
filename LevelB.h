#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 6;
    
    ~LevelB();
    
    void initialise(int player_health) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
