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

    struct SoundBuffer
    {
         string key;
         shared_ptr<sf::SoundBuffer> sf_soundBuffer;
         fs::path filePath;
    };

    class ResourceManager : public Serializable
    {
        friend class Scene;
            
    public:            
        static ResourceManager& Get();
        
        weak_ptr<Texture> LoadTexture(fs::path pathToTexture);
        weak_ptr<SoundBuffer> LoadSound(fs::path pathToSound);

        weak_ptr<Texture> GetTexture(string key);
        weak_ptr<SoundBuffer> GetSound(string key);

        list<Texture> GetAllTextures();
        list<SoundBuffer> GetAllSounds();

    private:

        ResourceManager() {};
        ~ResourceManager() {};
        ResourceManager(const ResourceManager &) = delete;
        ResourceManager &operator=(const ResourceManager &) = delete;

        json Serialize();
        void  Deserialize(json obj); 

        map<string, shared_ptr<Texture>> textures;
        map<string, shared_ptr<SoundBuffer>> sounds;

        string GenerateHash(string input);
        
    };
}
