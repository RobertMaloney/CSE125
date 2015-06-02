#include "GameSound.h"

sf::Sound	*GameSound::menumove		= NULL,
			*GameSound::menuconfirm		= NULL,
			*GameSound::menuback		= NULL,
			*GameSound::blobmove		= NULL,
			*GameSound::nom				= NULL,
			*GameSound::regburp			= NULL,
			*GameSound::bigburp			= NULL,
			*GameSound::jump			= NULL;

sf::Music	*GameSound::menubgm			= NULL,
			*GameSound::ingamebgm		= NULL;

sf::SoundBuffer	*GameSound::menumove_buf		= NULL,
				*GameSound::menuconfirm_buf		= NULL,
				*GameSound::menuback_buf		= NULL,
				*GameSound::blobmove_buf		= NULL,
				*GameSound::nom_buf				= NULL,
				*GameSound::regburp_buf			= NULL,
				*GameSound::bigburp_buf			= NULL,
				*GameSound::jump_buf			= NULL;

void GameSound::init()
{
	menumove_buf = new sf::SoundBuffer();
	menuconfirm_buf = new sf::SoundBuffer();
	menuback_buf = new sf::SoundBuffer();
	blobmove_buf = new sf::SoundBuffer();
	nom_buf = new sf::SoundBuffer();
	regburp_buf = new sf::SoundBuffer();
	bigburp_buf = new sf::SoundBuffer();
	jump_buf = new sf::SoundBuffer();

	menuconfirm = new sf::Sound();
	menumove = new sf::Sound();
	menuback = new sf::Sound();
	blobmove = new sf::Sound();
	nom = new sf::Sound();
	regburp = new sf::Sound();
	bigburp = new sf::Sound();
	jump = new sf::Sound();

	menubgm = new sf::Music();
	ingamebgm = new sf::Music();
	
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

	//MUSIC

	//menumusic
	loadMusic(menubgm, "../../media/sound/menubgm2.flac");

	//menumusic
	loadMusic(ingamebgm, "../../media/sound/ingamebgm2.flac");
}


void GameSound::loadSound(sf::Sound *sound, sf::SoundBuffer *buffer, std::string path)
{
	if (!buffer->loadFromFile(path))
		return;
	sound->setBuffer(*buffer);
}


void GameSound::loadMusic(sf::Music *music, std::string path)
{
	if (!music->openFromFile(path))
		return;
	music->setVolume(50);
	music->setLoop(true);
}