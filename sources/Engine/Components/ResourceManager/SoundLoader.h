#pragma once

#include<string>
#include<Engine/Components/Sound/Sound.h>

#include"ResourceLoader.h"

class SoundLoader : public ResourceLoader
{
public:
	SoundLoader();
	virtual ~SoundLoader();
	virtual BaseResourceInstance* load(const std::string& path, std::optional<std::any> options);
private:
	BaseResourceInstance* load_format_wav(const std::string& path, std::optional<std::any> options);
	BaseResourceInstance* load_format_ogg(const std::string& path, std::optional<std::any> options);
};