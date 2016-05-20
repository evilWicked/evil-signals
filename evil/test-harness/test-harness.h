#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>

#include "test-case.h"
#include "test-data-table.h"

//implementation bought in at the bottom of the file

namespace evil {

	class CTestHarness {

		typedef std::vector<int> t_widths;
		typedef std::list<CTestCase*> test_list;
		typedef test_list::iterator test_list_iter;

		const std::string mName;
		const std::string mLog;
		test_list mlistTests;

		CDataTable<std::string> mResultTable;

		bool mbStopOnError;
		bool mbUseStdOut;
		unsigned int muDotCount;

		t_widths maColWidths;//column widths of output

		///process and tidy up the results
		void formatResults();

		///truncate or expand columns to fit 
		void adjustCols();
		void dumpLog();
		void dumpConsole();
		void dump();

	public:

		///set name to NULL to not produce a file
		CTestHarness(const char *name,const char *log_name = "TEST_LOG.TXT", bool stopOnError = false, bool useStdOut = true);
		void add(const char *name, CTestCase::type_testfunc func);
		void run();
		void outDot();///outputs a dot to the console if being used
		~CTestHarness();

	public:
		//Hide and disable/default all constructors and destructors unless specifically overridden
		//this is a mnemonic to force me to think about things. if overridden they are commented
		//CTestHarness() = default;
		//~CTestHarness() = default;
		CTestHarness(const CTestHarness& rhs) = delete;
		CTestHarness& operator=(const CTestHarness& rhs) = delete;
		CTestHarness(CTestHarness&& other) = delete;
		CTestHarness& operator=(CTestHarness&& other) = delete;
	};

}

