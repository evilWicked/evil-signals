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
		~CTestHarness();
	};

}

#include "imp/test-harness.cpp"