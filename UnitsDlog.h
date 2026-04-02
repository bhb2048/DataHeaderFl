//
//	UnitsDlog.h - subclass of UnitsDlogUI for editing channel units
//	
//	15-nov-12  bhb
//	Modified:
//	
#ifndef UnitsDlog_h
#define UnitsDlog_h
#include "UI/UnitsDlogUI.h"

class UnitsDlog : public UnitsDlogUI
{
	public:
		UnitsDlog() {}
		virtual ~UnitsDlog() {}

		void initUnits( const char *unit1, const char *unit2);
};
#endif
