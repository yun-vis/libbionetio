//==============================================================================
// BioNetXML.cc
//	: macro definitions common to this package
//
//==============================================================================

#ifndef _BioNetIOXML_H               // beginning of header file
#define _BioNetIOXML_H               // notifying that this file is included

#include <map>
#include <vector>
#include <sstream>

#include "include/CommonClass.h"

//------------------------------------------------------------------------------
//	Macro Switches
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Macro definitions
//------------------------------------------------------------------------------
class BioNetIOXML
{
private:

	vector< BioNetIOMetabolite > meta;
	vector< BioNetIOReaction > react;
	map< string, BioNetIOSubsystem > sub;

public:
//------------------------------------------------------------------------------
//      Constructors
//------------------------------------------------------------------------------
	BioNetIOXML( void ) {}                         // constructor (default)
	virtual ~BioNetIOXML( void ) {}                // destructor

//------------------------------------------------------------------------------
//      Special functions
//------------------------------------------------------------------------------

	bool findMetabolite( string name, int &index );
    bool findSubsystems( string name, int &index );

	// xml
	void loadXML( string filename );
	void saveXML( string outputpath, string sysName );
	void exportXML( string outputpath );

	// sbml
	bool saveSBML( string outputpath, string sysName );
	void exportSBML( string filename );
};



#endif // _BioNetIOXML_H
