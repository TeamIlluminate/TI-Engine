#include "ResourceManager.h"

using namespace eng;

ResourceManager &ResourceManager::Get()
{
    static ResourceManager mngr;
    return mngr;
}

weak_ptr<Texture> ResourceManager::LoadTexture(fs::path pathToTexture)
{
    auto key = GenerateHash(pathToTexture);
    if (textures.find(key) != textures.end()) {
        return textures.at(key);
    }
    auto sf_texture = make_shared<sf::Texture>();
    weak_ptr<Texture> empty;
    if (sf_texture->loadFromFile(pathToTexture))
    {
        auto texture = make_shared<Texture>();
        texture->sf_texture = sf_texture;
        texture->filePath = pathToTexture;
        texture->key = GenerateHash(pathToTexture);
        textures.insert(pair<string, shared_ptr<Texture>>(texture->key, texture));
        return texture;
    }
    else
        return empty;
}

weak_ptr<SoundBuffer> ResourceManager::LoadSound(fs::path pathToSound)
{
    auto key = GenerateHash(pathToSound);
    if (sounds.find(key) != sounds.end()) {
        return sounds.at(key);
    }
    auto sf_soundBuffer = make_shared<sf::SoundBuffer>();
    weak_ptr<SoundBuffer> empty;
    if (sf_soundBuffer->loadFromFile(pathToSound))
    {
        auto soundBuffer = make_shared<SoundBuffer>();
        soundBuffer->sf_soundBuffer = sf_soundBuffer;
        soundBuffer->filePath = pathToSound;
        soundBuffer->key = GenerateHash(pathToSound);
        sounds.insert(pair<string, shared_ptr<SoundBuffer>>(soundBuffer->key, soundBuffer));
        return soundBuffer;
    }
    else
        return empty;
}

weak_ptr<Texture> ResourceManager::GetTexture(string key)
{
    return textures[key];
}
weak_ptr<SoundBuffer> ResourceManager::GetSound(string key)
{
    return sounds[key];
}

list<Texture> ResourceManager::GetAllTextures()
{

}

list<SoundBuffer> ResourceManager::GetAllSounds()
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
    for (auto pairTexture : textures)
    {
        js["textures"][i] = pairTexture.second->filePath;
        i++;
    }
    i = 0;
    for (auto pairSound : sounds)
    {
        js["sounds"][i] = pairSound.second->filePath;
        i++;
    }
    return js;
}

void ResourceManager::Deserialize(json obj)
{
    textures.clear();
    auto jTextures = obj["textures"];
    for (auto jTexture : jTextures)
    {
        LoadTexture(jTexture);
    }
    auto jSounds = obj["sounds"];
    for (auto jSound : jSounds)
    {
        LoadSound(jSound);
    }
}