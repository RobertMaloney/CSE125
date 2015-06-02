#include "GameSound.h"

#include <string>
#include <cstdlib>

sf::Sound	*GameSound::menumove		= NULL,
			*GameSound::menuconfirm		= NULL,
			*GameSound::menuback		= NULL,
			*GameSound::blobmove		= NULL,
			*GameSound::nom				= NULL,
			*GameSound::regburp			= NULL,
			*GameSound::bigburp			= NULL,
			*GameSound::jump			= NULL,

			*GameSound::ouch_arr[SOUND_OUCH_NUM] = {}
			;

sf::Music	*GameSound::menubgm			= NULL,
			*GameSound::menubgm2		= NULL,
			*GameSound::ingamebgm		= NULL,
			*GameSound::ingamebgm2		= NULL
			;

sf::SoundBuffer	*GameSound::menumove_buf		= NULL,
				*GameSound::menuconfirm_buf		= NULL,
				*GameSound::menuback_buf		= NULL,
				*GameSound::blobmove_buf		= NULL,
				*GameSound::nom_buf				= NULL,
				*GameSound::regburp_buf			= NULL,
				*GameSound::bigburp_buf			= NULL,
				*GameSound::jump_buf			= NULL,

				*GameSound::ouch_buf_arr[SOUND_OUCH_NUM] = {}
				;

void GameSound::init()
{
	menumove_buf	= new sf::SoundBuffer();
	menuconfirm_buf = new sf::SoundBuffer();
	menuback_buf	= new sf::SoundBuffer();
	blobmove_buf	= new sf::SoundBuffer();
	nom_buf			= new sf::SoundBuffer();
	regburp_buf		= new sf::SoundBuffer();
	bigburp_buf		= new sf::SoundBuffer();
	jump_buf		= new sf::SoundBuffer();
	for (int i = 0; i < SOUND_OUCH_NUM; i++) {
		ouch_buf_arr[i] = new sf::SoundBuffer();
	}

	menuconfirm		= new sf::Sound();
	menumove		= new sf::Sound();
	menuback		= new sf::Sound();
	blobmove		= new sf::Sound();
	nom				= new sf::Sound();
	regburp			= new sf::Sound();
	bigburp			= new sf::Sound();
	jump			= new sf::Sound();
	for (int i = 0; i < SOUND_OUCH_NUM; i++) {
		ouch_arr[i] = new sf::Sound();
	}

	menubgm			= new sf::Music();
	menubgm2		= new sf::Music();
	ingamebgm		= new sf::Music();
	ingamebgm2		= new sf::Music();
	
	//menumove
	loadSound(menumove, menumove_buf, "../../media/sound/menumove.wav");

	//menuconfirm
	loadSound(menuconfirm, menuconfirm_buf, "../../media/sound/menuconfirm.wav");

	//menuback
	loadSound(menuback, menuback_buf, "../../media/sound/menuback.wav");

	//blobmove
	loadSound(blobmove, blobmove_buf, "../../media/sound/blobmove.wav");
	blobmove->setLoop(true);

	//nom
	loadSound(nom, nom_buf, "../../media/sound/nom.wav");

	//regburp
	loadSound(regburp, regburp_buf, "../../media/sound/regburp.wav");

	//bigburp
	loadSound(bigburp, bigburp_buf, "../../media/sound/bigburp.wav");

	//jump
	loadSound(jump, jump_buf, "../../media/sound/jump.wav");

	//ouch
	loadSoundColl(ouch_arr, ouch_buf_arr, "../../media/sound/ouch", SOUND_OUCH_NUM);

	//MUSIC

	//menubgm1
	loadMusic(menubgm, "../../media/sound/menubgm.flac");

	//menubgm2
	loadMusic(menubgm2, "../../media/sound/menubgm2.flac");

	//ingamebgm
	loadMusic(ingamebgm, "../../media/sound/ingamebgm.flac");

	//ingamebgm2
	loadMusic(ingamebgm2, "../../media/sound/ingamebgm2.flac");
}


void GameSound::loadSound(sf::Sound *sound, sf::SoundBuffer *buffer, std::string path)
{
	if (!buffer->loadFromFile(path))
		return;
	sound->setBuffer(*buffer);
}


void GameSound::loadSoundColl(sf::Sound *sound[], sf::SoundBuffer *buffer[], std::string path, int size)
{
	for (int i = 0; i < size; i++) {
		int index = i + 1;
		if (!buffer[i]->loadFromFile(path + std::to_string(index) + ".wav"))
			return;
		sound[i]->setBuffer(*buffer[i]);
	}
}


void GameSound::loadMusic(sf::Music *music, std::string path)
{
	if (!music->openFromFile(path))
		return;
	music->setVolume(50);
	music->setLoop(true);
}


void GameSound::playOuch()
{
	ouch_arr[rand() % SOUND_OUCH_NUM]->play();
}