#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#define SFML_STATIC
#include <SFML/Audio.hpp>


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

	static void init();


private:
	static sf::SoundBuffer	*menumove_buf, 
							*menuconfirm_buf,
							*blobmove_buf,
							*nom_buf, 
							*regburp_buf, 
							*bigburp_buf, 
							*jump_buf;

	
};

#endif