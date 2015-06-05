#include "GameSound.h"

#include <string>
#include <cstdlib>

sf::Sound	*GameSound::menumove	= NULL,
			*GameSound::menuconfirm = NULL,
			*GameSound::menuback	= NULL,
			*GameSound::blobmove	= NULL,
			*GameSound::nom			= NULL,
			*GameSound::regburp		= NULL,
			*GameSound::bigburp		= NULL,
			*GameSound::jump		= NULL,
			*GameSound::spaceship	= NULL,
			*GameSound::loadingnow	= NULL,
			*GameSound::spaceship_loading = NULL,

			*GameSound::ouch_arr[SOUND_OUCH_NUM] = {}
;

sf::Music	*GameSound::menubgm		= NULL,
			*GameSound::menubgm2	= NULL,
			*GameSound::ingamebgm	= NULL,
			*GameSound::ingamebgm2	= NULL,
			*GameSound::loadingloop	= NULL
;

sf::SoundBuffer	*GameSound::menumove_buf	= NULL,
				*GameSound::menuconfirm_buf = NULL,
				*GameSound::menuback_buf	= NULL,
				*GameSound::blobmove_buf	= NULL,
				*GameSound::nom_buf			= NULL,
				*GameSound::regburp_buf		= NULL,
				*GameSound::bigburp_buf		= NULL,
				*GameSound::jump_buf		= NULL,
				*GameSound::spaceship_buf	= NULL,
				*GameSound::loadingnow_buf	= NULL,
				*GameSound::spaceship_loading_buf  = NULL,

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
	spaceship_buf	= new sf::SoundBuffer();
	loadingnow_buf	= new sf::SoundBuffer();
	spaceship_loading_buf = new sf::SoundBuffer();
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
	spaceship		= new sf::Sound();
	loadingnow		= new sf::Sound();
	spaceship_loading = new sf::Sound();
	for (int i = 0; i < SOUND_OUCH_NUM; i++) {
		ouch_arr[i] = new sf::Sound();
	}

	menubgm			= new sf::Music();
	menubgm2		= new sf::Music();
	ingamebgm		= new sf::Music();
	ingamebgm2		= new sf::Music();
	loadingloop		= new sf::Music();

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

	//spaceship
	loadSound(spaceship, spaceship_buf, "../../media/sound/spaceship.wav");

	//loadingnow
	loadSound(loadingnow, loadingnow_buf, "../../media/sound/loadingnow.wav");

	//loadingnow
	loadSound(spaceship_loading, spaceship_loading_buf, "../../media/sound/spaceship_loading.wav");

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

	//loadingbgm
	loadMusic(loadingloop, "../../media/sound/loadingloop.wav");
}


void GameSound::loadSound(sf::Sound *sound, sf::SoundBuffer *buffer, std::string path)
{
	if (!SOUND_ENABLE) {
		return;
	}
	if (!buffer->loadFromFile(path))
		return;
	sound->setBuffer(*buffer);
}


void GameSound::loadSoundColl(sf::Sound *sound[], sf::SoundBuffer *buffer[], std::string path, int size)
{
	if (!SOUND_ENABLE) {
		return;
	}
	for (int i = 0; i < size; i++) {
		int index = i + 1;
		if (!buffer[i]->loadFromFile(path + std::to_string(index) + ".wav"))
			return;
		sound[i]->setBuffer(*buffer[i]);
	}
}


void GameSound::loadMusic(sf::Music *music, std::string path)
{
	if (!SOUND_ENABLE) {
		return;
	}
	if (!music->openFromFile(path))
		return;
	music->setVolume(50);
	music->setLoop(true);
}


void GameSound::playOuch()
{
	if (!SOUND_ENABLE) {
		return;
	}
	ouch_arr[rand() % SOUND_OUCH_NUM]->play();
}


void GameSound::playLoading()
{
	if (!SOUND_ENABLE) {
		return;
	}
	spaceship_loading->play();
	loadingloop->play();
}


void GameSound::stopLoading()
{
	loadingloop->stop();
}