#include "Scene.h"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 3;
    

    ~LevelA();
    
    void initialise(int player_health) override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
