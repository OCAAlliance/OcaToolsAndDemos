/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The TestSuite class.
 */
#ifndef TEST_SUITE_H
#define TEST_SUITE_H

// ---- Include system wide include files ----
#include <vector>

// ---- Include local include files ----
#include "TestLogger.h"

// ---- Referenced classes and types ----
class BaseTestClass;
// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Holds all test cases.
 * Executes the tests on request.
 */
class TestSuite : private TestLogger
{
public:

    /**
     * Getter for the TestSuite singleton.
     *
     * @return  Reference to the TestSuite.
     */
    static ::TestSuite& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    /** 
     * Initialize by loading all known test cases.
     *
     * @return the result of the operation.
     */
    bool Initialize();

    /**
     * Teardown the testsuite. All test result will be lost.
     *
     * TODO
     */
    bool Teardown();

    /** 
     * Add test case to the list of test cases to run.
     *
     * @param[in] testCase  The testcase to add.
     */
    void AddTestCase(::BaseTestClass* testCase);

    /**
     * Execute all tests
     *
     * @param[in] stopOnError   True iff the test should be stopped on an error (Either test execution / test result).
     *
     * @return True iff executing the test succeeded.
     */
    bool ExecuteTests(bool stopOnError = false);

protected:
    /** Constructor */
    TestSuite();

    /** Destructor. */
    virtual ~TestSuite();

private:
    /** Typedef of a vector of test cases */
    typedef std::vector< ::BaseTestClass*> TestClassList;
    
    /** Test cases */
    TestClassList m_testCases;

    /** The base log path */
    std::string m_baseLogPath;
    /** Singleton instance of the server */
    static ::TestSuite* m_pSingleton;

    /** private copy constructor, no copying of object allowed */
    TestSuite(const ::TestSuite&);
    /** private assignment operator, no assignment of object allowed */
    ::TestSuite& operator=(const ::TestSuite&);
};

#endif //TEST_SUITE_H
