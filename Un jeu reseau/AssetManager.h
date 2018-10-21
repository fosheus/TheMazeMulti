#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AssetManager
{
public:
	AssetManager() {}
	~AssetManager() {}

	const sf::Texture& AssetManager::GetImage(const std::string& filename);
	const sf::Font& AssetManager::GetFont(const std::string & filename);
	const sf::SoundBuffer* AssetManager::GetSound(const std::string & filename);


private :

	std::map<std::string, sf::Texture > _images;
	std::map<std::string, sf::SoundBuffer> _sounds;
	std::map<std::string, sf::Font> _fonts;


	
};

