#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

// tinyxml
#include <tinyxml.h>
//#include <tinystr.h>

// sbml
#include <sbml/SBMLTypes.h>
#include <sbml/xml/XMLNode.h>

using namespace std;

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
//  Special functions
//------------------------------------------------------------------------------

bool BioNetIOXML::findSubsystems( string name, int &index )
{
    map< string, BioNetIOSubsystem >::iterator it = sub.begin();
    for( ; it != sub.end(); it++ ){

        if( it->second.name == name ){
            // cerr << it->second.name << " ?= " << name << endl;
            index = it->second.id;
            return true;
        }
    }

    return false;
}

bool BioNetIOXML::findMetabolite( string name, int &index )
{
	for( unsigned int i = 0; i < meta.size(); i++ ){

		if( meta[i].name == name ){
			index = i;
			// cerr << " meta[i].name = " << meta[i].name << " ?= " << name << endl;
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------------
//  Specific function declaration
//------------------------------------------------------------------------------
void BioNetIOXML::loadXML( string filename )
{
	// cerr << "file.c_str() = " << file.c_str() << endl;
	TiXmlDocument xmlDoc( filename.c_str() );

	xmlDoc.LoadFile();

	if( xmlDoc.ErrorId() > 0 ) {
		cerr << "ErrorId = " << xmlDoc.ErrorDesc() << endl;
		return;
	}

	TiXmlElement* rootMetabolites = xmlDoc.RootElement();
	TiXmlElement* rootReactions = rootMetabolites->NextSiblingElement();
	TiXmlElement* rootSubsystems = rootReactions->NextSiblingElement();
	//cerr << "rootMetabolites = " << rootMetabolites->Value() << endl;
	//cerr << "rootReactions = " << rootReactions->Value() << endl;

	if( !rootMetabolites || !rootReactions ) return;

	// reading metabolites
	for( TiXmlElement* metabolite = rootMetabolites->FirstChildElement(); metabolite; metabolite = metabolite->NextSiblingElement() ){

		BioNetIOMetabolite newMeta;
		//cerr << "metabolite = " << metabolite->Value() << endl;

		for( TiXmlElement* child = metabolite->FirstChildElement(); child; child = child->NextSiblingElement() ){

			string strname = child->Value();
			//cerr << strname << " = " << child->GetText() << endl;
			if( strname == "name" ){
				if( child->GetText() != NULL )
					newMeta.name = child->GetText();
				else
					newMeta.description = "";
			}
			if( strname == "formula" ){
				if( child->GetText() != NULL )
					newMeta.formula = child->GetText();
				else
					newMeta.description = "";
			}
			if( strname == "description" ){
				if( child->GetText() != NULL )
					newMeta.description = child->GetText();
				else
					newMeta.description = "";
			}

		}
		meta.push_back( newMeta );
	}
	//cerr << " nMetabolites = " << meta.size() << endl;

	// reading reactions
	for( TiXmlElement* reaction = rootReactions->FirstChildElement(); reaction; reaction = reaction->NextSiblingElement() ){

		BioNetIOReaction newReact;
		//cerr << "reaction = " << reaction->Value() << endl;

		for( TiXmlElement* child = reaction->FirstChildElement(); child; child = child->NextSiblingElement() ){

			string strname = child->Value();
			//cerr << "child = " << strname << endl;
			if( strname == "id" ){
				newReact.id = child->GetText();
			}
			if( strname == "name" ){
				newReact.name = child->GetText();
			}
			if( strname == "reactant" ){
				newReact.reactantVec.push_back( child->GetText() );
			}
			if( strname == "product" ){
				newReact.productVec.push_back( child->GetText() );
			}
			if( strname == "reversible" ){
				newReact.reversible = child->GetText();
			}
			if( strname == "subsystem" ){
				newReact.subsystem = child->GetText();

				map< string, BioNetIOSubsystem >::iterator it = sub.find( newReact.subsystem );
				if( it != sub.end() ){
					it->second.id++;
				}
				else{
					BioNetIOSubsystem s;
					s.id = 1;
                    s.name = newReact.subsystem;
                    sub.insert( pair< string, BioNetIOSubsystem >( newReact.subsystem, s ) );
				}
			}

		}

		react.push_back( newReact );
	}
	//cerr << " nReactions = " << react.size() << endl;

	// reading subsystems if existed
	if( rootSubsystems != NULL ){

        for( TiXmlElement* subsys = rootSubsystems->FirstChildElement(); subsys; subsys = subsys->NextSiblingElement() ){

            string id, name, color, x, y;

            for( TiXmlElement* child = subsys->FirstChildElement(); child; child = child->NextSiblingElement() ){

                string strname = child->Value();
                //cerr << "child = " << strname << endl;
                if( strname == "id" ){
                    id = child->GetText();
                }
                if( strname == "name" ){
                    name = child->GetText();
                }
                if( strname == "x" ){
                    x = child->GetText();
                }
                if( strname == "y" ){
                    y = child->GetText();
                }
                if( strname == "color" ){
                    color = child->GetText();
                }
            }

            cerr << "name = " << name << endl;
            map< string, BioNetIOSubsystem >::iterator itS = sub.find( name );
            itS->second.id = atoi( id.c_str() );
            itS->second.name = name;
            itS->second.x = x;
            itS->second.y = y;
            itS->second.color = color;
        }
        cerr << " nSubsystem = " << sub.size() << endl;
        }
}

void BioNetIOXML::saveXML( string outputpath, string sysName )
{
	TiXmlDocument xmlDoc;

	TiXmlNode *metaboliteElement = xmlDoc.InsertEndChild( TiXmlElement( "metabolites" ) );
	TiXmlNode *reactionElement = xmlDoc.InsertEndChild( TiXmlElement( "reactions" ) );
	TiXmlNode *subsysElement = xmlDoc.InsertEndChild( TiXmlElement( "subsystems" ) );

	// searching metabolites in the subsystems
	map< string, int > subMetaboliteMap;
	for( unsigned int i = 0; i < react.size(); i++ ){

		// searching metabolites in the subsystem - reactant
		for( unsigned int j = 0; j < react[i].reactantVec.size(); j++ ){
			int index = -1;
			string name = react[i].reactantVec[j];

			//cerr << "name = " << name << endl;
			if( sysName == react[i].subsystem ){

				string buf; // have a buffer string
				stringstream ss( name ); // insert the string into a stream

				// a vector to hold our words
				vector< string > tokens;
				while (ss >> buf) tokens.push_back( buf );

				// check info in the metabolite list
				findMetabolite( tokens[ tokens.size()-1 ], index );
				//cerr << "  name = " << name << " index = " << index << endl;
				//cerr << "sysName = " << sysName << endl;
				assert( index != -1 );
				meta[ index ].freq += 1;
				subMetaboliteMap.insert( pair< string, int >( name, index ) );
			}
		}

		// searching metabolites in the subsystem - product
		for( unsigned int j = 0; j < react[i].productVec.size(); j++ ){
			int index = -1;
			string name = react[i].productVec[j];

			//cerr << "name = " << name << endl;
			if( sysName == react[i].subsystem ){

				string buf; // have a buffer string
				stringstream ss( name ); // insert the string into a stream

				// a vector to hold our words
				vector< string > tokens;
				while (ss >> buf) tokens.push_back( buf );

				findMetabolite( tokens[ tokens.size()-1 ], index );
				//cerr << " name = " << name << " index = " << index << endl;
				//cerr << " sysName = " << sysName << endl;
				assert( index != -1 );
				meta[ index ].freq += 1;
				subMetaboliteMap.insert( pair< string, int >( name, index ) );
			}
		}
	}

	// printing the metabolites
	for( map< string, int >::iterator it = subMetaboliteMap.begin(); it != subMetaboliteMap.end(); ++it ){

		int index = it->second;
		TiXmlNode* newNode = new TiXmlElement( "metabolite" );

		newNode->InsertEndChild( TiXmlElement( "name" ) )->InsertEndChild( TiXmlText( meta[index].name ) );
		newNode->InsertEndChild( TiXmlElement( "formula" ) )->InsertEndChild( TiXmlText( meta[index].formula ) );
		newNode->InsertEndChild( TiXmlElement( "description" ) )->InsertEndChild( TiXmlText( meta[index].description ) );

		metaboliteElement->InsertEndChild( *newNode );
	}

	// printing the reactions
	for( unsigned int i = 0; i < react.size(); i++ ){

		if( sysName == react[i].subsystem ){

			TiXmlNode* newNode = new TiXmlElement( "reaction" );

			newNode->InsertEndChild( TiXmlElement( "id" ) )->InsertEndChild( TiXmlText( react[i].id ) );
			newNode->InsertEndChild( TiXmlElement( "name" ) )->InsertEndChild( TiXmlText( react[i].name ) );
			for( unsigned int j = 0; j < react[i].reactantVec.size(); j++ ){
				newNode->InsertEndChild( TiXmlElement( "reactant" ) )->InsertEndChild( TiXmlText( react[i].reactantVec[j] ) );
			}
			for( unsigned int j = 0; j < react[i].productVec.size(); j++ ){
				newNode->InsertEndChild( TiXmlElement( "product" ) )->InsertEndChild( TiXmlText( react[i].productVec[j] ) );
			}
			newNode->InsertEndChild( TiXmlElement( "reversible" ) )->InsertEndChild( TiXmlText( react[i].reversible ) );
			newNode->InsertEndChild( TiXmlElement( "subsystem" ) )->InsertEndChild( TiXmlText( react[i].subsystem ) );

			reactionElement->InsertEndChild( *newNode );
		}
	}

	// printing the subsystem
	map< string, BioNetIOSubsystem >::iterator itS = sub.begin();
	for( ; itS != sub.end(); itS++ ){

		if( itS->second.name == sysName ){

			TiXmlNode* newNode = new TiXmlElement( "subsystem" );

			newNode->InsertEndChild( TiXmlElement( "id" ) )->InsertEndChild( TiXmlText( to_string( itS->second.id ) ) );
			newNode->InsertEndChild( TiXmlElement( "name" ) )->InsertEndChild( TiXmlText( itS->second.name ) );
			newNode->InsertEndChild( TiXmlElement( "color" ) )->InsertEndChild( TiXmlText( itS->second.color ) );
			newNode->InsertEndChild( TiXmlElement( "x" ) )->InsertEndChild( TiXmlText( itS->second.x ) );
			newNode->InsertEndChild( TiXmlElement( "y" ) )->InsertEndChild( TiXmlText( itS->second.y ) );

			subsysElement->InsertEndChild( *newNode );
		}
	}

	string outputname = outputpath + "/" + sysName + ".xml";
	xmlDoc.SaveFile( outputname.c_str() );
}

void BioNetIOXML::exportXML( string outputpath )
{
	ofstream        ofs( "../filelist.txt" );

	if ( !ofs ) {
		cerr << "Cannot open the target file : " << "../filelist.txt" << endl;
		assert( false );
	}

	// export the file
	map< string, BioNetIOSubsystem >::iterator itB = sub.end();
	itB--;
	for( map< string, BioNetIOSubsystem >::iterator it = sub.begin(); it != sub.end(); ++it ){

		saveXML( outputpath, it->first );

		string filename = it->first;
		//string filename = it->first + ".xml";
		if( it == itB )
			ofs << filename;
		else
			ofs << filename << endl;

		cerr << "Writing content to " << "../" + filename << endl;
	}

	// export the metabolite frequency
	ofstream        off( "../metabolite_frequency.txt" );

	if ( !ofs ) {
		cerr << "Cannot open the target file : " << "../metabolite_frequency.txt" << endl;
		assert( false );
	}

	for( unsigned int i = 0; i < meta.size(); i++ ){
		if( i == meta.size()-1 ){
			off << meta[ i ].name << "\t" << meta[ i ].freq;
			//cerr << meta[ i ].name << "\t" << meta[ i ].freq;
		}
		else{
			off << meta[ i ].name << "\t" << meta[ i ].freq << endl;
			//cerr << meta[ i ].name << "\t" << meta[ i ].freq << endl;
		}
	}

	cerr << " nMetabolites = " << meta.size() << endl;
	cerr << " nReactions = " << react.size() << endl;
	cerr << " nSubsystems = " << sub.size() << endl;
}

bool BioNetIOXML::saveSBML( string outputpath, string sysName )
{
	//SBMLDocument *sbmlDocPtr = new SBMLDocument( 2, 4 );  // level 2, version 4, newest support by CellDesigner
    SBMLDocument *sbmlDocPtr = new SBMLDocument( 3, 1 );    // newest support of libsbml 5.12, support reaction compartment

    Model* modelPtr = sbmlDocPtr->createModel();
    modelPtr->setId( sysName );
    modelPtr->setMetaId( sysName );

    // printing the subsystem
    map< string, BioNetIOSubsystem >::iterator itS = sub.begin();
    for( ; itS != sub.end(); itS++ ){

        // cerr << "sysName = " << sysName << " ?= " << itS->second.name << endl;
        if( itS->second.name == sysName ){

            Compartment* compartment = modelPtr->createCompartment();
            compartment->setId( "C"+to_string( itS->second.id ) );  // need to begin with an alphabet, not a number
            compartment->setName( itS->second.name );
        }
    }

	// searching metabolites in the subsystems
	map< string, int > subMetaboliteMap;
	for( unsigned int i = 0; i < react.size(); i++ ){

		// searching metabolites in the subsystem - reactant
		for( unsigned int j = 0; j < react[i].reactantVec.size(); j++ ){
			int index = -1;
			string name = react[i].reactantVec[j];

			//cerr << "name = " << name << endl;
			if( sysName == react[i].subsystem ){

				string buf; // have a buffer string
				stringstream ss( name ); // insert the string into a stream

				// a vector to hold our words
				vector< string > tokens;
				while (ss >> buf) tokens.push_back( buf );

				// check info in the metabolite list
				findMetabolite( tokens[ tokens.size()-1 ], index );
				//cerr << "  name = " << name << " index = " << index << endl;
				//cerr << "sysName = " << sysName << endl;
				assert( index != -1 );
				meta[ index ].freq += 1;
				subMetaboliteMap.insert( pair< string, int >( name, index ) );
			}
		}

		// searching metabolites in the subsystem - product
		for( unsigned int j = 0; j < react[i].productVec.size(); j++ ){
			int index = -1;
			string name = react[i].productVec[j];

			//cerr << "name = " << name << endl;
			if( sysName == react[i].subsystem ){

				string buf; // have a buffer string
				stringstream ss( name ); // insert the string into a stream

				// a vector to hold our words
				vector< string > tokens;
				while (ss >> buf) tokens.push_back( buf );

				findMetabolite( tokens[ tokens.size()-1 ], index );
				//cerr << " name = " << name << " index = " << index << endl;
				//cerr << " sysName = " << sysName << endl;
				assert( index != -1 );
				meta[ index ].freq += 1;
				subMetaboliteMap.insert( pair< string, int >( name, index ) );
			}
		}
	}

    // printing the metabolites
    for( map< string, int >::iterator it = subMetaboliteMap.begin(); it != subMetaboliteMap.end(); ++it ){

        int index = it->second;

        Species * species = modelPtr->createSpecies();
        species->setId( "S" + to_string( index ) );
        species->setName( meta[index].name );
        // meta[index].formula
        // meta[index].description
    }

	// printing the reactions
	for( unsigned int i = 0; i < react.size(); i++ ){

		if( sysName == react[i].subsystem ){

		    Reaction * reaction = modelPtr->createReaction();
            //reaction->setId( "R"+ react[i].id );
            reaction->setId( react[i].id );
            reaction->setName( react[i].name );
            bool rev;
            if( react[i].reversible == "true" ) rev = true;
            else rev = false;
            reaction->setReversible( rev );

            int sID;
            findSubsystems( react[i].subsystem, sID );

            reaction->setCompartment( "C"+to_string( sID ) );
            // cerr << "compartment->getId() = " << compartment->getId() << endl;
            // cerr << "sys = " << reaction->getCompartment() << endl;
            // reaction->getCompartment() support from Level 3

			for( unsigned int j = 0; j < react[i].reactantVec.size(); j++ ){

                int index;
                findMetabolite( react[i].reactantVec[j], index );

                SpeciesReference* reference = reaction->createReactant();
                reference->setSpecies( "S" + to_string( index ) );
                reference->setId( "SpeciesReference" );
                //cerr << "reactant = " << index << endl;
            }
			for( unsigned int j = 0; j < react[i].productVec.size(); j++ ){

                int index;
                findMetabolite( react[i].productVec[j], index );

                SpeciesReference* reference = reaction->createProduct();
                reference->setSpecies( "S" + to_string( index ) );
                reference->setId( "SpeciesReference" );
                //cerr << "product = " << index << endl;
			}
		}
	}

	string outputname = outputpath + "/" + sysName + ".sbml";
	// cerr << "sbml level = " << sbmlDocPtr->getLevel() << endl;
	// cerr << "sbml version = " << sbmlDocPtr->getVersion() << endl;

	SBMLWriter sbmlWriter;
	bool result = sbmlWriter.writeSBML( sbmlDocPtr, outputname );

	if ( result ) return true;
	else return false;
}

void BioNetIOXML::exportSBML( string outputpath )
{
	ofstream        ofs( "../filelist.txt" );

	if ( !ofs ) {
		cerr << "Cannot open the target file : " << "../filelist.txt" << endl;
		assert( false );
	}

	// export the file
	map< string, BioNetIOSubsystem >::iterator itB = sub.end();
	itB--;
	for( map< string, BioNetIOSubsystem >::iterator it = sub.begin(); it != sub.end(); ++it ){

		saveSBML( outputpath, it->first );

		string filename = it->first;
		//string filename = it->first + ".xml";
		if( it == itB )
			ofs << filename;
		else
			ofs << filename << endl;

		// cerr << "Writing content to " << "../" + filename << endl;
	}

	// export the metabolite frequency
	ofstream        off( "../metabolite_frequency.txt" );

	if ( !ofs ) {
		cerr << "Cannot open the target file : " << "../metabolite_frequency.txt" << endl;
		assert( false );
	}

	for( unsigned int i = 0; i < meta.size(); i++ ){
		if( i == meta.size()-1 ){
			off << meta[ i ].name << "\t" << meta[ i ].freq;
			//cerr << meta[ i ].name << "\t" << meta[ i ].freq;
		}
		else{
			off << meta[ i ].name << "\t" << meta[ i ].freq << endl;
			//cerr << meta[ i ].name << "\t" << meta[ i ].freq << endl;
		}
	}

	cerr << " nMetabolites = " << meta.size() << endl;
	cerr << " nReactions = " << react.size() << endl;
	cerr << " nSubsystems = " << sub.size() << endl;
}