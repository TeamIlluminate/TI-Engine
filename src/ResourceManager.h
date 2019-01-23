#include "Utils.h"
#include <experimental/filesystem>
#include "Serializable.h"
#pragma once

namespace eng
{
    namespace fs = experimental::filesystem;

    struct Texture
    {
        string key;
        shared_ptr<sf::Texture> sf_texture;
        fs::path filePath;
    };

    struct Sound
    {
        // string key;
        // sf::Sound sf_sound;
        // fs::path filePath;
    };

    class ResourceManager : public Serializable
    {
        friend class Scene;
            
    public:            
        static ResourceManager& Get();
        
        weak_ptr<Texture> LoadTexture(fs::path pathToTexture);
        Sound LoadSound(fs::path pathToSound);

        weak_ptr<Texture> GetTexture(string key);
        sf::Sound GetSound(string key);

        list<Texture> GetAllTextures();
        list<Sound> GetAllSounds();

    private:

        ResourceManager() {};
        ~ResourceManager() {};
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        json Serialize();
        void  Deserialize(json obj); 

        map<string, shared_ptr<Texture>> textures;
        map<string, Sound> sounds;

        string GenerateHash(string input);
        
    };
}
