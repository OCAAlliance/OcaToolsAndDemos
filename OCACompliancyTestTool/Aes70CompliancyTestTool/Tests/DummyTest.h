/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The Dummy test class.
 */
#ifndef DUMMY_TEST_H
#define DUMMY_TEST_H

 // ---- Include system wide include files ----

 // ---- Include local include files ----
#include "../TestFramework/BaseTestClass.h"

 // ---- Referenced classes and types ----

 // ---- Helper types and constants ----

 // ---- Helper functions ----

 // ---- Class Definition ----


class DummyTest : public BaseTestClass
{
public:
    /** Constructor */
    DummyTest() :
        BaseTestClass("DummTest")
    {

    }

    /** Implement interface **/
    virtual bool ExecuteTest()
    {
        // Add some dummy test logs to test the functionality
        AddTestResult("Test TestResult");
        AddTestResult("With Params %d", 1);
        AddTestLog("Test TestLog");
        AddTestLog("With Params %d", 1);

        // Seems very succesfull.
        SetTestResult(true);
        
        return true;
    }

protected:

private:
};

#endif //DUMMY_TEST_H