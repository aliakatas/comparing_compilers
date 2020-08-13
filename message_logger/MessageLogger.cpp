#include "MessageLogger.h"

#include <iostream>

//////////////////////////////////////////////////
MessageLogger::MessageLogger(bool atCMD)
{
	std::cout << "MessageLogger object here...\n";
	ierr = 0;
	write2Files = false;
	write2cmd = atCMD;
	nFiles = 0;
}

//////////////////////////////////////////////////
MessageLogger::~MessageLogger()
{
	ierr = 0;

	writeOutput("Destroying message writing object... \n");

	// Just add a new line to separate from subsequent messages.
	if (write2cmd)
		std::cout << "\n";

	nFiles = 0;

	// iterate through all open files and close them
	// before resizing to 0.
	for (auto it = 0; it < myFids.size(); it++)
		myFids[it].close();

	myFids.resize(0);
	myFiles.resize(0);
	std::cout << "MessageLogger object destroyed!\n";
}

//////////////////////////////////////////////////
void MessageLogger::addFile(const std::string str)
{
	nFiles++;

	try
	{
		// Open and add the identifier
		myFids.push_back(std::ofstream(str, std::ofstream::out));

		// Store the name of the file 
		myFiles.push_back(str);

		write2Files = true;
	}
	catch (...)
	{
		ierr = -1;
	}
}

//////////////////////////////////////////////////
void MessageLogger::openFiles()
{
	for (auto it = 0; it < myFiles.size(); it++)
		myFids.push_back(std::ofstream(myFiles[it], std::ofstream::out));
}

//////////////////////////////////////////////////
void MessageLogger::closeFiles()
{
	for (auto it = 0; it < myFids.size(); it++)
		myFids[it].close();
}

//////////////////////////////////////////////////
bool MessageLogger::writeOutput(const std::string str, const std::string ending)
{
	// Show the message in command line
	if (write2cmd)
	{
		try
		{
			std::cout << str << ending;
		}
		catch (...)
		{
			ierr = -2;
			return false;
		}

	}

	// Show the message in all the necessary files
	if (write2Files)
	{
		// iterate through files and write message!
		for (auto it = 0; it < myFids.size(); it++)
		{
			try
			{
				myFids[it] << str << ending;
			}
			catch (...)
			{
				ierr = -3;
				return false;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////
short MessageLogger::getErrorStatus()
{
	return ierr;
}

//////////////////////////////////////////////////
std::string MessageLogger::getErrorString()
{
	switch (ierr)
	{
	case (-999):
		return "Failed to properly initialise the object.";
	case (-1):
		return "Failed to add file as an output unit.";
	case (-3):
		return "Failed to write output to file.";
	case (-2):
		return "Failed to write output to command line.";
	case (0):
		return "Success.";
	default:
		return "Unknown status, consider terminating process now.";
	}
}