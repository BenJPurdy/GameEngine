#pragma once

#include "Logging/Log.h"
#include <fmod.hpp>
#include <random>

namespace GameEngine
{
	const char* FMOD_ErrorString(FMOD_RESULT);
	class AudioEngine
	{
	public:
		//lets play a sound from a component
		static void play(FMOD::Sound* sound)
		{
			LOG_TRACE("Playing audio from component");
			FMOD::Channel* channel;
			//AudioEngine::channels.push_back(channel);
			FMOD_RESULT err = AudioEngine::system->playSound(sound, nullptr, false, &channel);
			if (err != FMOD_OK)
			{
				LOG_ERROR("Failed to play sound in component: {0}", FMOD_ErrorString(err));
			}
			channel->setPitch(1.0f);
		}
		static inline FMOD::System* system;
		std::string assetFolder = "../GameEngine/assets/Audio/";

        //fn from the fmod examples provided with the lib
		//gives nicer error messages
		
		static inline std::vector<FMOD::Sound*> sounds = {};
		static inline std::vector<FMOD::Channel*> channels;
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

			LOG_TRACE("Loaded {0} sounds from assets", sounds.size());
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
			LOG_TRACE("new sounds size {0}", sounds.size());

		}

		//this function is called from the scripting API to play a sound
		void playSound(int index)
		{
			LOG_TRACE("Playing sound from script");
			//if (index >= sounds.size())
			//{
			//	LOG_ERROR("Sound index {0} is bigger than the maximum index of sounds {1}", index, sounds.size());
			//}
			FMOD::Channel* newChannel;
			channels.push_back(newChannel);
			system->playSound(sounds[0], nullptr, false, &channels.back());
			channels.back()->setPitch(1.0f);
		}
		
		//void playTestSound()
		//{
		//	bool isPlaying;
		//	channel->isPlaying(&isPlaying);
		//	if (!isPlaying)
		//	{
		//		float r = ((float)rand() / (float)RAND_MAX) + 0.5f;
		//		r = std::max(r, 0.5f);
		//		float currPitch;
		//		LOG_TRACE("Playing audio");
		//		float idx = ((float)rand() / RAND_MAX);
		//		int index = round(idx);
		//		//
		//		index = 0;
		//
		//		system->playSound(sounds[index], nullptr, false, &channel);
		//		FMOD_RESULT res = channel->setPitch(r);
		//		channel->getPitch(&currPitch);
		//		LOG_TRACE("Plaing Sound: {0}, pitch is {1}", idx, currPitch);
		//	}
		//}

		void update()
		{
			system->update();
			for (size_t i = 0; i < channels.size(); i++)
			{
				bool channelPlaying;
				channels[i]->isPlaying(&channelPlaying);
				if (!channelPlaying)
				{
					channels.erase(channels.begin() + i);
				}
			}
		}

		void destory()
		{
			for (auto& i : sounds)
			{
				i->release();
			}
			system->release();
		}

		~AudioEngine()
		{
			//system->release();
		}
	};
}

