/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Compliancy test tool documentation
 */

/**
 * @mainpage AES70 Compliancy Test Tool Console User Manual
 * The AES70 Compliancy Test Tool is a verification tool for compliancy of devices supporting AES70 to the AES70 specifications. 
 * The tool offers a way to check if AES70 devices are interoperable (on a basic level) with other AES70 devices.
 * A console window to run the tool can be opened by selecting the tool in the Windows start menu. 
 * This documentation describes how to use the tool to test compliancy of an AES70 device and contains the following sections:
 * - @ref octtcli
 * - @ref octtcompatibility
 * - @ref octtlogging
 */

/**
 * @page octtcli Command Line Options
 * The following command line options are available:
 *
 * Option                               | Description
 * -------------------------------------|-------------
 * -d &lt;device name&gt;               | Mandatory parameter that specifies the name of the device on which the AES70 Compliancy Test Tool will run the compliancy test.
 * -s &lt;supported services&gt;        | Mandatory parameter that specifies the services which are expected to be supported by the device. Options are tcp, tcp-sec, udp. Multiple can be appended with \| as seperator.
 * -v &lt;aes70 version&gt;             | Mandatory parameter that specifies the Aes70 specification version to be tested for.
 * -l &lt;loglevel&gt;                  | Optional parameter that can be used to indicate the loglevel. Supported are: INFO, WARNING and ERROR.
 * -t &lt;device type&gt;               | Optional parameter that can be used to specify the device type. By default it uses the basic type, SECURE, STREAM can added. Multiple can be appended with \| as seperator.
 *
 * Examples of valid commands are:<br />
 * <code>Aes70CompliancyTestTool.exe -t MyFirstOCADevice -s UDP|TCP -s AES70-2018</code><br />
 * <code>Aes70CompliancyTestTool.exe -t MyFirstOCADevice -s UDP|TCP -s AES70-2018 -l INFO -t SECURE|STREAMING </code><br />
 * <code>Aes70CompliancyTestTool.exe -t MyFirstOCADevice -s TCP-SEC|TCP -s AES70-2015  -t SECURE</code>
 */

/**
 * @page octtcompatibility Compatibility
 * The AES70 Compliancy Test Tool is both backward and forward compatible.<br /><br />
 * <b>Backward compatibility:</b> By definition AES70 will remain backward compatible due to the rules that have been set inside the specification 
 *                        all classes are versioned, properties methods and events of approved versions may never be removed and the class
 *                        version is raised if something is added. The AES70 Compliancy Test Tool will use a specific attribute in the XML file containing the
 *                        OCC class definitions that will make sure that the AES70 Compliancy Test Tool knows what parts belong to what class version. The AES70 Compliancy Test Tool 
 *                        will check the class version of each object of a device and only test the parts belonging to that class version 
 *                        This way the AES70 Compliancy Test Tool offers full backward compatibility, objects of older class versions will not lead to failing tests.<br /><br />
 * <b>Forward compatibility:</b> This is handled by updating the XML file containing the OCC class definitions (ReferenceOCCMembers.xml). 
 *                        The AES70 Compliancy Test Tool is kept up-to-date with any changes that are made to the AES70 specification (specifically the OCC part). 
 *                        The ReferenceOCCMembers.xsd is the XML schema definition that can be used to verify the correct syntax of ReferenceOCCMembers.xml.  
 *                        As soon as changes have been approved a new version of the AES70 Compliancy Test Tool containing an updated XML file will be released, 
 *                        thereby guaranteeing forward compatibility (i.e. always being up-to-date with the specification).<br /><br />
 *
 */

/**
 * @page octtlogging Logging
 * The test run log file of the AES70 Compliancy Test Tool will contain detailed logging of the compliancy test run that is executed on the device.
 * The test result log file will contain the overall test result.
 * For each test a new log file and test result file will be generated. The name and location of this log file are fixed to %programdata%\OCAAlliance\CompliancyTestTool
 * The log file will contain detailed 
 * test results for all executed test steps. The following example shows an excerpt of the log file:
 * <pre>
Starting Test : OCA Service Discovery

Testing DNS-SD for resolved service CAP6-000181._oca._tcp.local.
   Port for service CAP6-000181._oca._tcp.local. validated (port 55555)
   At least 2 txt records must be present. Result success
   Successfully validated TXT records
Testing basic connection for resolved service CAP6-000181._oca._tcp.local.
   Established connection (1) for service CAP6-000181._oca._tcp.local.
   OcaBlock.GetMembersRecursive returns status 0, nr members 271. Expecting OK. Test Passed
   OcaDeviceManager.GetManagers returns status 0, nr managers 7. Expecting OK or NotImplemented. Test Passed
                                  ...
                                  ...
                                  ...
Test Result Summary for device TestDevice:
   5 tests executed
   5 tests passed
   0 tests failed
Device TestDevice is considered to be COMPLIANT to AES70-20xx
 * </pre>
 *
 * Next to the logging in the log file, the AES70 Compliancy Test Tool will also write a summary to the console. The following example shows what this console output may look like:
 * <pre>
Test Result Summary for device TestDevice:
   5 tests executed
   5 tests passed
   0 tests failed
Device TestDevice is considered to be COMPLIANT to AES70-20xx
 * </pre>
 */
