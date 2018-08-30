//==============================================================================
// CommonClass.cc
//	: macro definitions common to this package
//
//==============================================================================

#ifndef _CommonClass_H               // beginning of header file
#define _CommonClass_H               // notifying that this file is included

#include <map>
#include <vector>
#include <sstream>

//------------------------------------------------------------------------------
//	Macro Switches
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Macro definitions
//------------------------------------------------------------------------------
class BioNetIOMetabolite
{
public:
	string	name;
	string	formula;
	string	description;
	unsigned int freq;			// frequency of the metabolite

    BioNetIOMetabolite(){ freq = 0; };	// default constructor
};

class BioNetIOReaction
{
public:
	string id;              // reaction id
	string name;            // reaction name
	vector< string > reactantVec;
	vector< string > productVec;
	string reversible;      // reversible
	string subsystem;       // subsystem
};

class BioNetIOSubsystem
{
public:
	string name;        // subsystem name
	unsigned int id;    // subsystem id
	string x;           // x
	string y;           // y
	string color;       // color
};

#endif // _CommonClass_H
