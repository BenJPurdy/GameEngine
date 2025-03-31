#pragma once

#include "Logging/Log.h"
#include <fmod.hpp>
#include <random>

namespace GameEngine
{
	class AudioEngine
	{
	public:
		FMOD::System* system = nullptr;
		std::string assetFolder = "../GameEngine/assets/Audio/";

        //fn from the fmod examples provided with the lib
		static const char* FMOD_ErrorString(FMOD_RESULT);
		std::vector<FMOD::Sound*> sounds;
		FMOD::Channel* channel;
		bool isSoundPlaying = false;

		void init()
		{
			FMOD_RESULT err = FMOD::System_Create(&system);
			if (err != FMOD_OK)
			{
				LOG_ERROR("Failed to initialise FMOD: {0}", FMOD_ErrorString(err));
			}
			err = system->init(512, FMOD_INIT_NORMAL, 0);
			if (err != FMOD_OK)
			{
				LOG_ERROR("Failed to intialise FMOD system: {0}", FMOD_ErrorString(err));
			}

			//std::string testSound = assetFolder + "audio_test.wav";
			//system->createSound(testSound.c_str(), )
			addSound("audio_test.wav");
			addSound("ADNS.wav");
		}

		void addSound(std::string sound)
		{
			std::string pathToSound = assetFolder + sound;
			FMOD::Sound* newSound = nullptr;
			FMOD_RESULT err = system->createSound(pathToSound.c_str(), FMOD_LOOP_OFF, nullptr, &newSound);
			if (err != FMOD_OK)
			{
				LOG_ERROR("Failed to create sound: {0}", FMOD_ErrorString(err));
			}
			sounds.push_back(newSound);

		}

		void playTestSound()
		{
			bool isPlaying;
			channel->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				float r = ((float)rand() / (float)RAND_MAX) + 0.5f;
				r = std::max(r, 0.5f);
				float currPitch;
				LOG_TRACE("Playing audio");
				float idx = ((float)rand() / RAND_MAX);
				int index = round(idx);
				//
				index = 0;

				system->playSound(sounds[index], nullptr, false, &channel);
				FMOD_RESULT res = channel->setPitch(r);
				channel->getPitch(&currPitch);
				LOG_TRACE("Plaing Sound: {0}, pitch is {1}", idx, currPitch);
			}
		}

		void update()
		{
			system->update();
		}

		~AudioEngine()
		{
			//system->release();
		}
	};
}

