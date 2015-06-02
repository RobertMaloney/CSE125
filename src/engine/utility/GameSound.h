#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#define SFML_STATIC
#include <SFML/Audio.hpp>
//#include <SFML/System.hpp>
#pragma comment(lib, "winmm.lib") // SO weird I need this for music

class GameSound 
{
public:
	static sf::Sound	*menumove,
						*menuconfirm,
						*menuback,
						*blobmove,
						*nom,
						*regburp,
						*bigburp,
						*jump;

	static sf::Music	*menubgm,
						*ingamebgm;

	static void init();


private:
	//music doesn't need buffer
	static sf::SoundBuffer	*menumove_buf, 
							*menuconfirm_buf,
							*menuback_buf,
							*blobmove_buf,
							*nom_buf, 
							*regburp_buf, 
							*bigburp_buf, 
							*jump_buf
							;

	static void loadSound(sf::Sound *sound, sf::SoundBuffer *buffer, std::string path);
	static void loadMusic(sf::Music *music, std::string path);
	
};

#endif