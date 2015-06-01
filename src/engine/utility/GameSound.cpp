#include "GameSound.h"

sf::Sound	*GameSound::menumove		= NULL,
			*GameSound::menuconfirm		= NULL,
			*GameSound::blobmove		= NULL,
			*GameSound::nom				= NULL,
			*GameSound::regburp			= NULL,
			*GameSound::bigburp			= NULL,
			*GameSound::jump			= NULL;

sf::Music	*GameSound::menumusic		= NULL,
			*GameSound::ingamemusic		= NULL;

sf::SoundBuffer	*GameSound::menumove_buf		= NULL,
				*GameSound::menuconfirm_buf		= NULL,
				*GameSound::blobmove_buf		= NULL,
				*GameSound::nom_buf				= NULL,
				*GameSound::regburp_buf			= NULL,
				*GameSound::bigburp_buf			= NULL,
				*GameSound::jump_buf			= NULL;

void GameSound::init()
{
	menumove_buf = new sf::SoundBuffer();
	menuconfirm_buf = new sf::SoundBuffer();
	blobmove_buf = new sf::SoundBuffer();
	nom_buf = new sf::SoundBuffer();
	regburp_buf = new sf::SoundBuffer();
	bigburp_buf = new sf::SoundBuffer();
	jump_buf = new sf::SoundBuffer();
	//menumusic_buf = new sf::SoundBuffer();
	//ingamemusic_buf = new sf::SoundBuffer();

	menuconfirm = new sf::Sound();
	menumove = new sf::Sound();
	blobmove = new sf::Sound();
	nom = new sf::Sound();
	regburp = new sf::Sound();
	bigburp = new sf::Sound();
	jump = new sf::Sound();

	menumusic = new sf::Music();
	ingamemusic = new sf::Music();
	
	
	if (!menumove_buf->loadFromFile("../../media/sound/menumove.wav"))
		return;
	menumove->setBuffer(*menumove_buf);

	if (!menuconfirm_buf->loadFromFile("../../media/sound/menuconfirm.wav"))
		return;
	menuconfirm->setBuffer(*menuconfirm_buf);

	if (!blobmove_buf->loadFromFile("../../media/sound/blobmove.wav"))
		return;
	blobmove->setBuffer(*blobmove_buf);
	blobmove->setLoop(true);

	if (!nom_buf->loadFromFile("../../media/sound/nom.wav"))
		return;
	nom->setBuffer(*nom_buf);

	if (!regburp_buf->loadFromFile("../../media/sound/regburp.wav"))
		return;
	regburp->setBuffer(*regburp_buf);

	if (!bigburp_buf->loadFromFile("../../media/sound/bigburp.wav"))
		return;
	bigburp->setBuffer(*bigburp_buf);

	if (!jump_buf->loadFromFile("../../media/sound/jump.wav"))
		return;
	jump->setBuffer(*jump_buf);

	//MUSIC
	if (!menumusic->openFromFile("../../media/sound/menubgm2.flac"))
		return;
	menumusic->setVolume(50);
	menumusic->setLoop(true);

	if (!ingamemusic->openFromFile("../../media/sound/ingamebgm2.flac"))
		return;
	ingamemusic->setVolume(50);
	ingamemusic->setLoop(true);
	
}