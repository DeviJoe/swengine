#include "SoundLoader.h"

#include "ResourceLoadingException.h"
#include <filesystem>
#include <STB/stb_vorbis.c>

#define DR_WAV_IMPLEMENTATION
#include <dr_libs/dr_wav.h>


SoundLoader::SoundLoader()
{
}

SoundLoader::~SoundLoader()
{
}

BaseResourceInstance * SoundLoader::load(const std::string & path, std::optional<std::any> options)
{
	const std::string expansion = path.substr(path.find_last_of('.')+1);
	
	if (expansion == "wav") {
		return load_format_wav(path, options);
	}
	else if (expansion == "ogg") {
		return load_format_ogg(path, options);
	}
	else
	{
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "File type is wrong", __FILE__, __LINE__, __FUNCTION__);
	}

}

BaseResourceInstance * SoundLoader::load_format_wav(const std::string & path, std::optional<std::any> options)
{
	drwav* pWav = drwav_open_file(path.c_str());

	if (pWav == NULL) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "File loading error", __FILE__, __LINE__, __FUNCTION__);
	}

	if (pWav->channels > 2) {
		drwav_close(pWav);
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "The sound file has more than 2 audio channels", __FILE__, __LINE__, __FUNCTION__);
	}

	unsigned long long audioFileSize = (unsigned long long)pWav->totalPCMFrameCount * pWav->channels * sizeof(int16_t);

	std::byte* pSampleData = new std::byte[audioFileSize];
	drwav_read_pcm_frames_s16(pWav, pWav->totalPCMFrameCount, (drwav_int16*)pSampleData);
	unsigned int sampleRate = pWav->sampleRate;

	Sound* loaded = new Sound(pSampleData, (size_t)pWav->channels, audioFileSize, sampleRate);
	drwav_close(pWav);
	delete pSampleData;

	return new ResourceInstance<Sound>(loaded);
}

BaseResourceInstance * SoundLoader::load_format_ogg(const std::string & path, std::optional<std::any> options)
{
	int channels, audioFileSize, sampleRate;
	std::byte *decoded;

	if (!std::filesystem::exists(path)) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "File doesn't exist", __FILE__, __LINE__, __FUNCTION__);
	}

	audioFileSize = stb_vorbis_decode_filename(path.c_str(), &channels, &sampleRate, (short**)&decoded);

	if (channels > 2) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "The sound file has more than 2 audio channels", __FILE__, __LINE__, __FUNCTION__);
	}

	Sound* loaded = new Sound(decoded, (size_t)channels, audioFileSize*channels * sizeof(short), sampleRate);
	delete decoded;

	return new ResourceInstance<Sound>(loaded);
}
