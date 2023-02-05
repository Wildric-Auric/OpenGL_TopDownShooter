#include <fstream>
#include "Builder.h"
#include "Utilities.h"
#include "Maths.h"


std::vector<std::string> Builder::IncludeDir = {};

std::vector<std::string> Builder::LibsDir = {};

std::vector<std::string> Builder::staticLibs = {};

std::vector<std::string> Builder::objs = {};                                                                                                      
                                   

std::vector<std::string> Builder::PreprocessorMacros = {
    "GLEW_STATIC",
    "WIN32",
    "WIN_32",
    "_CRT_SECURE_NO_WARNINGS",
    "_CONSOLE",
    "_MBCS"   
    //_USRDLL and _WINDLL for DLL
};

std::string Builder::runtimeLib = "/MDd";

void Builder::InitCompilation(std::ofstream* ofs, std::string outputDir) {
    *ofs << "@echo off\n";
    *ofs << " if not defined DevEnvDir (call vcvars32)\n";

    //Getting it's name
    *ofs << "cl /EHsc ";
    *ofs << runtimeLib << " ";  //Runtime library
    *ofs << "/c ";

    for (std::string MAC : PreprocessorMacros) 
        *ofs << " /D" << MAC << " ";
    *ofs << "^\n";
    *ofs << " /Fo\""<< outputDir << "\" ";

    bool b = 0;
    //Adding include dirs
    for (std::string dir : IncludeDir) {
        if (b) {
            *ofs << "^\n ";
        }
        *ofs << "/I " << "\"" << dir << "\"";
        b = 1;
    };
}

//Output dir: only directory
void Builder::GenerateCompilationData(std::ofstream* ofs, std::string file, std::string outputDir) {
    std::string filename = "";
    std::string extension = "";
    GetFileName(file, &filename, &extension);
    //Adding cpp file
    *ofs << " \"" << file << "\"";
    *ofs << "^\n";
}

//Compiles the file passed as first argument
void Builder::CompileInd(std::string file, std::string outputDir) {
    //preprocessor macros
    std::ofstream ofs("Builder.bat");
    if (!ofs) return;
    InitCompilation(&ofs, outputDir);
    GenerateCompilationData(&ofs, file, outputDir);
    ofs << " > log.txt";
    ofs.close();
}

//Compiles all files in objs vector
void Builder::Compile(std::string outputDir) {
    std::ofstream ofs("Builder.bat");
    if (!ofs) return;
    InitCompilation(&ofs, outputDir);
    for (std::string file : objs)
        GenerateCompilationData(&ofs, file, outputDir);
    ofs << " > log.txt";
    ofs.close();
}

//output is directory + name
void Builder::Link(std::string output, bool isDll) {
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << " if not defined DevEnvDir (call vcvars32)\n";
    ofs << "LINK ";
    for (std::string obj : objs) {
        ofs << "\"" << obj << "\"" << "^\n ";
    }
    if (isDll)
        ofs <<  "/DLL ";
    for (std::string dir : LibsDir) {
        ofs << "/LIBPATH:" << "\"" <<dir<< "\""<< "^\n ";
    }
    for (std::string lib : staticLibs) {
        ofs << lib << "^\n ";
    }
    ofs << "/OUT:"  << output << " > log.txt";
    ofs.close();
}

void Builder::GenLib(std::string output) {
    std::ofstream ofs("Builder.bat");
    ofs << "@echo off\n";
    ofs << " if not defined DevEnvDir (call vcvars32)\n";
    ofs << "lib ";
    for (std::string obj : objs) {
        ofs <<"\n" << obj << "^\n";
    }
    ofs << " /OUT:" << output;
}

//Loc is directory where 
void Builder::InitScripts(std::string scriptList, std::string scriptManager) {
    //TODO::Wrapper of ifstream and ofstream with error handling
    //Reading used scripts names and writing to ScriptManager.cpp
    std::ifstream ifs(scriptList);
    if (!ifs) {
        std::cout << "Can't open scripts file"<< std::endl;
        return; 
    }
    std::ofstream ofs;

    std::string scriptMap = "";
    std::string scripts   = "";
    std::string fileName  = "";
    std::string root      = "";
    //Iterating over lines in scripts' NWlist
    for (std::string line; std::getline(ifs, line);) {
        GetFileName(line, &fileName, nullptr, &root);
        scriptMap += "\n  {\"" + fileName + "\"," + fileName + "::GetScript" + "},";
        objs.push_back(root + fileName + ".cpp"); //Add user's script to files that should be compiled
        scripts +=  "#include \"" + root + fileName + ".h\"\n";
    };
    if (scriptMap.size() > 0) scriptMap.pop_back();
    ofs.close();
    ifs.close();
    //Building scriptManager map
    Vector2<std::string> parts = Vector2<std::string>("", "");
    Vector2<std::string> del = Vector2<std::string>("BEG_PPP","END_PPP");
    std::ifstream ifs0(scriptManager);
    if (!ifs0) {
        std::cout << "Can't open ScriptManager.cpp file" << std::endl;
        return;
    }
    int j = 0;
    for (std::string line; std::getline(ifs0, line);) {
        if (line.find("BEG_PPP") != std::string::npos) {
            parts.x += "//BEG_PPP\n";
            j++;
            continue;
        }
        if (line.find("END_PPP") != std::string::npos) {
            parts.y += "//END_PPP\n";
            j++;
            continue;
        }
        if (j!=1) *parts[min(j, 1)] += line + "\n";
    }
    ifs0.close();
    std::ofstream ofs0(scriptManager);
    ofs0 << parts.x;
    ofs0 << scripts;
    ofs0 << "std::map<std::string, Scriptable* (*)(GameObject*)> ScriptManager::ScriptsMap = {\n";
    ofs0 << scriptMap <<"\n};\n" << parts.y;
    ofs0.close();
}

//Returns a vector containing all paths in a NWlist file
std::vector<std::string> Builder::GetDirs(std::string path) {
    std::fstream stream(path); //TODO::ERROR checking
    std::vector<std::string> vec;
    for (std::string line = ""; std::getline(stream, line); vec.push_back(line));
    return vec;
};


void Builder::Build() {
    //InitScripts();
}