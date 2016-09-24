/*
 * Copyright (c) 2016
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Process.h"
#include "Poco/NumberParser.h"
#include <iostream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;


class ProcessKillerApp: public Application
{
public:
	ProcessKillerApp(): 
		_helpRequested(false),
		_friendly(false)
	{
	}

protected:		
	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "Display help information on command line arguments.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ProcessKillerApp>(this, &ProcessKillerApp::handleHelp)));

		options.addOption(
			Option("friendly", "f", "Kindly ask application to shut down.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ProcessKillerApp>(this, &ProcessKillerApp::handleFriendly)));
	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		stopOptionsProcessing();
	}
	
	void handleFriendly(const std::string& name, const std::string& value)
	{
		_friendly = true;
	}
			
	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("[options] <pid> ...");
		helpFormatter.setHeader("A utility application to kill processes.");
		helpFormatter.setFooter("Note that the friendly option only works with applications using Poco::Util::ServerApplication::waitForTerminationRequest().");
		helpFormatter.format(std::cout);
	}
	
	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested || args.empty())
		{
			displayHelp();
		}
		else
		{
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
			{
				Poco::Process::PID pid = Poco::NumberParser::parseUnsigned(*it);
				if (_friendly)
					Poco::Process::requestTermination(pid);
				else
					Poco::Process::kill(pid);
			}
		}
		return Application::EXIT_OK;
	}
		
private:
	bool _helpRequested;
	bool _friendly;
};


POCO_APP_MAIN(ProcessKillerApp)
