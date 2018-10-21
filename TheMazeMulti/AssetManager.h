#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AssetManager
{
public:
	AssetManager() {}
	~AssetManager() {}

	const sf::Texture& GetImage(const std::string& filename);
	const sf::Font& GetFont(const std::string & filename);
	const sf::SoundBuffer* GetSound(const std::string & filename);


private :

	std::map<std::string, sf::Texture > _images;
	std::map<std::string, sf::SoundBuffer> _sounds;
	std::map<std::string, sf::Font> _fonts;


	
};

