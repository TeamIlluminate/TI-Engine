#include "ResourceManager.h"

using namespace eng;

ResourceManager& ResourceManager::Get()
{
    static ResourceManager mngr;
    return mngr;
}

weak_ptr<Texture> ResourceManager::LoadTexture(fs::path pathToTexture)
{
    auto sf_texture = make_shared<sf::Texture>();
    weak_ptr<Texture> empty;

    // if(!textures[pathToTexture])
    // {
        if(sf_texture->loadFromFile(pathToTexture))
        {
            auto texture = make_shared<Texture>();
            texture->sf_texture = sf_texture;
            texture->filePath = pathToTexture;
            texture->key = GenerateHash(pathToTexture);
            textures.insert(pair<string, shared_ptr<Texture>>(texture->key, texture));
            return textures[texture->key];
        }
    // }
    else return empty;
}

Sound ResourceManager::LoadSound(fs::path pathToTexture)
{
}

weak_ptr<Texture> ResourceManager::GetTexture(string key)
{
    return textures[key];
}
sf::Sound ResourceManager::GetSound(string key)
{
}

list<Texture> ResourceManager::GetAllTextures()
{
}
list<Sound> ResourceManager::GetAllSounds()
{
}

string ResourceManager::GenerateHash(string input)
{
    return input;
}

json ResourceManager::Serialize()
{
    json js;
    int i = 0;        
    for (auto pairTexture : textures) {
        js["textures"][i] = pairTexture.second->filePath; 
        i++;       
    }    
    return js;
}

void ResourceManager::Deserialize(json obj)
{
    textures.clear();
    auto jTextures = obj["textures"];
    for(auto jTexture : jTextures)
    {
        LoadTexture(jTexture);
    }
}