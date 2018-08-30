#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <tinyxml.h>
//#include <tinystr.h>

using namespace std;

#include "include/CommonClass.h"
#include "include/BioNetIOXML.h"

//------------------------------------------------------------------------------
//  Macro definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Variables for OpenGL functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Main function
//------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    string inputname = "";
    string outputpath = "";
    string format = "sbml";

    //cerr << argc << endl;
    for( int i = 1; i < argc; ){

        //cerr << argv[i] << ", " << argv[i+1] << endl;
        string str = argv[i];
        if( str == "-f" ) format = argv[i+1];
        if( str == "-i" ) inputname = argv[i+1];
        if( str == "-o" ) outputpath = argv[i+1];
        i += 2;
    }

    //inputname = "../input.xml";
    //outputpath = "../output";
    //string inputname = "../data/metabolites.xml";
    //string inputname = "small-metabolites.xml";
    cerr << "inputname = " << inputname << endl;
    cerr << "outputpath = " << outputpath << endl;
    cerr << "format = " << format << endl;

    BioNetIOXML xml;

	// load the xml file
	xml.loadXML( inputname );

	if( format == "xml" ){

        // export the separated xml files
        xml.exportXML( outputpath );
	}
	else if( format == "sbml" ){

        // export the separated sbml files
        xml.exportSBML( outputpath );
    }
	else{
        cerr << "Wrong file format... please specify \"-f sbml\" or \"-f xml\"..." << endl;
        return 0;
	}

	cerr << "libbiobetio finished successfully..." << endl;

    return 0;
}
