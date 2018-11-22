/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The Base class for tests.
 */
#ifndef TEST_LOGGER_H
#define TEST_LOGGER_H

// ---- Include system wide include files ----
#include <algorithm>
#include <vector>
// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Logger class
 */
class TestLogger 
{
public:

    /**
     * Get the test name
     *
     * @return The test name.
     */
    virtual const std::string& GetTestName() const;


    /** Vector of strings */
    typedef std::vector< std::string> StringVector;

    /**
     * Get the human readable test results.
     *
     * @return List with test results.
     */
    const StringVector& GetTestResults();

    /**
     * Get the test logs.
     *
     * @return List with test logs.
     */
    const StringVector& GetTestLogs();

    /**
    * Append the logs to the given filename.
    *
    * @param[in] bTestResults   True for test results, false for test logs 
    * @param[in] fileName       The filename to write to.
    *
    * @return True iff successfull.
    */
    bool AppendToFile(bool bTestResults, const std::string& fileName);



protected:
    /**
     * Constructor
     *
     * @param[in] testName  The name of the test
     */
    TestLogger(const std::string& testName);

    /**
     * Add a test result
     *
     * @param[in] text  Text to add
     * @Param[in] ...   Arguments
     */
    void AddTestResult(const char* text, ...);

    /**
     * Add a test log
     *
     * @param[in] text  Text to add
     * @Param[in] ...   Arguments
     */
    void AddTestLog(const char* text, ...);


private:
    /** The test name */
    std::string                             m_testName;
    /** Trace the test result */
    StringVector                            m_testResult;
    /** Trace the test log */
    StringVector                            m_testLog;

    /** private copy constructor, no copying of object allowed */
    TestLogger(const ::TestLogger&);
    /** private assignment operator, no assignment of object allowed */
    TestLogger& operator=(const ::TestLogger&);
};

#endif //TEST_LOGGER_H
