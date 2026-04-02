//
//	ChannelNamesDlog.h - subclass of ChannelNamesDlogUI for editing channel names
//	
//	23-oct-12  bhb
//	Modified:
//	
#ifndef ChannelNamesDlog_h
#define ChannelNamesDlog_h
#include "UI/ChannelNamesDlogUI.h"

typedef unsigned int uint;

class ChannelNamesDlog : public ChannelNamesDlogUI
{
	public:
		ChannelNamesDlog() {}
		virtual ~ChannelNamesDlog() {}

		void initChanNames( std::string *names, uint n);
};
#endif