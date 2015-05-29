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
						*blobmove,
						*nom,
						*regburp,
						*bigburp,
						*jump;

	static sf::Music	*menumusic,
						*ingamemusic;

	static void init();


private:
	static sf::SoundBuffer	*menumove_buf, 
							*menuconfirm_buf,
							*blobmove_buf,
							*nom_buf, 
							*regburp_buf, 
							*bigburp_buf, 
							*jump_buf
							
							//*menumusic_buf,
							//*ingamemusic_buf
							;

	
};

#endif