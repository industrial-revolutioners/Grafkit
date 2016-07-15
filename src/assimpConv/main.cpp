/**
	A quick and dirtz thool that loads, converts and saves models 
	to the native format of assimp.

	@author Caiwan
*/

#include "Arguments.hpp"

#include "assimp/Importer.hpp"
#include "assimp/Exporter.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/matrix4x4.h"


using namespace std;
using namespace ideup;

int main(int argc, char* argv[])
{
	Arguments args;

	args.add("help", 'h').description("Shows this help message.").flag(true);
	args.add("input", 'i').description("Input filename").required(true);
	args.add("output", 'o').description("Output filename").required(true);
	args.add("coords").description("Swap coordinate system axes. (xyz, xzy, ... )");
	args.add("textures").description("Strip texture filenames").flag(true);

	// parse args
	if (!args.evaluate(argc, argv)) {
		cout << args.getErrorMessage() << endl;
		return 1;
	}

	// print help
	if (argc == 1 || args.get("help").isFound()) {
		cout << args.getHelpMessage() << endl;
		return 0;
	}
 
	string inFileName = args.get("input").value();
	string outFileName = args.get("output").value();
	string outExtension;

	// strip file extension
	{
		string::size_type n;
		string s = outFileName;
		//do 
		{
			n = s.find_last_of('.');
			if (n != string::npos) {
				s = s.substr(n + 1);
			}
		} 
		//while (n != string::npos);
		outExtension = s;
	}

	//
	Assimp::Importer aiImporter;
	aiScene const * scene = aiImporter.ReadFile(inFileName.c_str(),
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_ConvertToLeftHanded |
		0);

	if (scene == nullptr) {
		cout << "Could not load model file" << inFileName << endl;
		cout << aiImporter.GetErrorString() << endl;
		return 1;
	}

	// save 
	Assimp::Exporter aiExporter;

	if (aiExporter.Export(scene, outExtension, outFileName) != aiReturn_SUCCESS) {
		cout << "Could not save model file" << endl;
		cout << aiExporter.GetErrorString() << endl;
		return 1;
	}

	return 0;
}