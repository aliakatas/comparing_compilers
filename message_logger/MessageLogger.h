#pragma once

#include <fstream>
#include <string>
#include <vector>

class MessageLogger
{
public:

	MessageLogger(bool atCMD = false);
	
	~MessageLogger();

	void addFile(const std::string str);

	bool writeOutput(const std::string str = " ", const std::string ending = "\n");
	
	short getErrorStatus();

	std::string getErrorString();

private:
	short ierr;										//!< General status of the object (what could possibly go wrong?!)
	bool write2Files;								//!< Informative variable whether it is writing to files or not
	bool write2cmd;									//!< Informative variable whether it is writing to command line or not
	int nFiles;										//!< Number of files to write to
	std::vector<std::ofstream> myFids;				//!< Array of file identifiers where messages go
	std::vector<std::string> myFiles;				//!< Array of file names where messages go

	void openFiles();

	void closeFiles();
};


