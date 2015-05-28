#include "GameSound.h"

sf::Sound	*GameSound::menumove,
			*GameSound::menuconfirm,
			*GameSound::blobmove		= NULL,
			*GameSound::nom				= NULL,
			*GameSound::regburp			= NULL,
			*GameSound::bigburp			= NULL,
			*GameSound::jump			= NULL;

sf::SoundBuffer	*GameSound::menumove_buf,
				*GameSound::menuconfirm_buf,
				*GameSound::blobmove_buf		= NULL,
				*GameSound::nom_buf				= NULL,
				*GameSound::regburp_buf			= NULL,
				*GameSound::bigburp_buf			= NULL,
				*GameSound::jump_buf			= NULL;

void GameSound::init()
{
	//menumove_buf = new sf::SoundBuffer();
	//menuconfirm_buf = new sf::SoundBuffer();

	//menuconfirm = new sf::Sound();
	//menumove = new sf::Sound();

	if (!menumove_buf->loadFromFile("../../../media/sound/menumove.wav"))
		return;
	menumove->setBuffer(*menumove_buf);

	if (!menuconfirm_buf->loadFromFile("../../../media/sound/menuconfirm.wav"))
		return;
	menuconfirm->setBuffer(*menuconfirm_buf);

	if (!blobmove_buf->loadFromFile("../../../media/sound/blobmove.wav"))
		return;
	blobmove->setBuffer(*blobmove_buf);

	if (!nom_buf->loadFromFile("../../../media/sound/nom.wav"))
		return;
	nom->setBuffer(*nom_buf);

	if (!regburp_buf->loadFromFile("../../../media/sound/regburp.wav"))
		return;
	regburp->setBuffer(*regburp_buf);

	if (!bigburp_buf->loadFromFile("../../../media/sound/bigburp.wav"))
		return;
	bigburp->setBuffer(*bigburp_buf);

	if (!jump_buf->loadFromFile("../../../media/sound/jump.wav"))
		return;
	jump->setBuffer(*jump_buf);
}