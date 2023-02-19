#pragma once
#include <string>
#include <vector>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN 

template<typename T> 
void ExtendVector(std::vector<T>* a, std::vector<T> b) {
	for (auto c : b) {
		a->push_back(c);
	}
}


std::vector<std::string> GetNWlist(std::string path);

//Next functions use windows api; LATETODO:: Add their equivelent for mac, linux etc...
//#ifdef WIN

/*See Interface.cpp to know these functions role. I could have used only one loop to get number and string but it's cleaner when done like
* like this and think it won't affect too much engine performance.
*/

struct DllHandle {
	DllHandle(const char* filename);
	~DllHandle();
	const void* Get() const;

private:
	void* h;
};


void* GetDllFunction(DllHandle* dll, const char* functionName);

std::vector<int> GetRecusivelyFilesNumber(const std::string& directory);
//DevNote: filesystem could be used here if C++ is minimum +17; since my intention is to use only
//C++ 11 I did not use it; instead I use windows api; which makes the application not cross plaform for now
std::vector<std::string> GetDirFiles(const std::string& directory);

std::vector<std::string> GetRecusivelyDirFiles(const std::string& directory);

int AccumulateChildren(std::vector<int>* a, std::vector<int>* b, int index = 0);

std::string GetCurrentDir();

std::string GetExePath();

std::string GetFile(const char* type = "Text Files\0*.txt\0*.*\0");

//Returns filename + extension
std::string GetFileName(std::string path, std::string* bFilename = nullptr, std::string* bExtension = nullptr, std::string* bRoot = nullptr);

bool FileCopy(std::string dest, std::string src, bool failIfExists = 0);

bool FileExists(std::string dir);

bool Exec(std::string cmd);

bool FileDelete(std::string name);

bool FileMove(std::string dest, std::string source, bool failIfExists = 0);