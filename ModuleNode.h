//
//	ModuleNode.h - FlNode subclass for AModule's
//
//	11-nov-05  bhb	split from AModule.h
//	Modified:
//	31-aug-07  bhb	fltk v2, convert to use FlNode instead of VkNode
//	
#ifndef MODULENODE_H
#define MODULENODE_H
#include <Fltk/FlNode.h>
#include <Data/AModule.h>

//////////////////////////////////////////////////////////////////////////
//	ModuleNode class - subclass VkNode so EditAMod graph nodes can point to AModule
//////////////////////////////////////////////////////////////////////////
class ModuleNode : public FlNode
{
	public:
		ModuleNode( AModule *am, FlGraph *g, const char *label = NULL)
			: FlNode( g, label), _am(am) {}
		~ModuleNode() {}

		AModule *		aModule()	{ return _am;	}
		virtual void	showControls()	
						{ if ( _am->ctrlDlog() == NULL) createModuleControls( _am);
							_am->showControls();	}
		void		print();
		ModuleControls *	createModuleControls( AModule *am);
		
	protected:
		AModule *	_am;
		
};
#endif
