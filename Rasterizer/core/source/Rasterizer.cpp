#include "../../../AmbiRenderer/core/Renderer.hpp"
//#include "../include/Config.hpp"

using std::shared_ptr;
using std::make_shared;

enum class SceneEnum;
class SceneManager;

class Rasterizer : public BaseRenderer {
public:
	virtual bool init_renderer(shared_ptr<Config> config) override;
	virtual bool render() override;
	//virtual bool load_scene(SceneEnum sceneEnum) override;

private:


private:
	//shared_ptr<Config> m_config;
	//shared_ptr<Camera> m_camera;
	shared_ptr<SceneManager> m_scene_manager;
};
