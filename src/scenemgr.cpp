#include <sge/scenemgr.hpp>

using namespace std;

namespace sge
{
    SGEScene::SGEScene() : root_node(nullptr) {}
    SGEScene::~SGEScene()
    {
        if (root_node != nullptr)
        {
            delete root_node;
        }
    }

    SGENode *SGEScene::get_root_node() const
    {
        return root_node;
    }

    SGESceneManager::SGESceneManager(SGEngine *engine) : current_scene(nullptr), engine(engine) {}

    SGESceneManager::~SGESceneManager()
    {
        if (current_scene != nullptr)
        {
            current_scene->unload(engine);
        }
    }

    void SGESceneManager::add_scene(string const &name, SGEScene *scene)
    {
        scenes[name] = scene;
    }

    void SGESceneManager::switch_to_scene(string const &name)
    {
        if (current_scene != nullptr)
        {
            if (current_scene->get_root_node() != nullptr)
            {
                current_scene->get_root_node()->send_exit_tree(engine);
            }

            current_scene->unload(engine);
        }

        current_scene = scenes[name];
        current_scene->load(engine);

        if (current_scene->get_root_node() != nullptr)
        {
            current_scene->get_root_node()->send_enter_tree(engine);
        }
    }

    bool SGESceneManager::event_handler(SDL_Event *event)
    {
        if (current_scene != nullptr && current_scene->get_root_node() != nullptr)
        {
            return current_scene->get_root_node()->send_input(engine, event);
        }

        return true;
    }

    void SGESceneManager::process_handler(Uint32 delta)
    {
        if (current_scene != nullptr && current_scene->get_root_node() != nullptr)
        {
            current_scene->get_root_node()->send_process(engine, delta);
        }
    }

    void SGESceneManager::draw_handler()
    {
        if (current_scene != nullptr && current_scene->get_root_node() != nullptr)
        {
            current_scene->get_root_node()->send_draw(engine);
        }
    }
}