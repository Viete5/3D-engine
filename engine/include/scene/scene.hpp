#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "sceneObject.hpp"
#include "camera.hpp"
#include "light.hpp"

namespace engine::platform {
class Input;
}

namespace engine::scene {

struct SceneUpdateContext {
    float delta_time;
    float elapsed_time;
    const engine::platform::Input& input;
};

class Scene {
public:
    Scene();
    Scene(const std::vector<SceneObject>& objects, const Camera& camera, const std::vector<Light>& lights);
    Scene(const Scene& other) = delete;
    Scene& operator=(const Scene& other) = delete;
    virtual ~Scene() = default;

    virtual void update(const SceneUpdateContext& context) = 0;

    std::vector<SceneObject>& get_objects();
    Camera& get_camera();
    std::vector<Light>& get_lights();
    
    const std::vector<SceneObject>& get_objects() const;
    const Camera& get_camera() const;
    const std::vector<Light>& get_lights() const;

    void set_objects(const std::vector<SceneObject>& new_objects);
    void set_camera(const Camera& new_camera);
    void set_lights(const std::vector<Light>& new_lights);

protected:
    std::vector<SceneObject> objects;
    Camera camera;
    std::vector<Light> lights;
};

}

#endif // SCENE_HPP
