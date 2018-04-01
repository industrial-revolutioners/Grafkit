#include "stdafx.h"
#include "animation.h"

using namespace Grafkit;

/* ============================================================================================== */

void Grafkit::Animation::_serialize(Archive & ar)
{
	PERSIST_STRING(ar, m_name);
}

// Serialize??? 

Grafkit::Animation::Track::Track(const char * name, const std::vector<std::string> channelNames)
	: m_name(name)
{
	if (!channelNames.empty()) {
		for (size_t a = 0; a < channelNames.size(); ++a) {
			CreateChannel(channelNames.at(a));
		}

	}
}

Grafkit::Animation::Track::Track(const char * name, const char * initials)
	: m_name(name)
{
	if (initials) {
		size_t i = 0;
		while (initials[i]) {
			CreateChannel(std::string(1, initials[i]));
			i++;
		}
	}
}

size_t Grafkit::Animation::Track::CreateChannel(const char * name)
{
	m_channels.push_back(new Channel(name));
	return m_channels.size() - 1;

}

size_t Grafkit::Animation::Track::CreateChannel(std::string name)
{
	m_channels.push_back(new Channel(name));
	return m_channels.size() - 1;
}

