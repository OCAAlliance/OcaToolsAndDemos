/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */
using Microsoft.Deployment.WindowsInstaller;
using System;
using System.Globalization;
using System.Runtime.InteropServices;
using System.ServiceProcess;

namespace OCAAlliance.CommonChecks
{
    public static class CustomActions
    {
        #region Private constants
        /// <summary>
        /// The name of the bonjour service to look for
        /// </summary>
        private const string BONJOUR_SERVICE_NAME = "Bonjour service";
        #endregion

        #region Public methods
        /// <summary>
        /// Performs a check whether bonjour is installed and active.
        /// </summary>
        /// <param name="session">The installation session.</param>
        /// <returns>The result of the custom actions.</returns>
        [CustomAction]
        public static ActionResult CheckBonjour(Session session)
        {
            ActionResult result = ActionResult.Failure;

            int counter = 0;
            bool bContinue = true;

            if (null != session)
            {
                session.Log("CheckBonjour: start");

                while (bContinue)
                {
                    session.Log(string.Format(CultureInfo.InvariantCulture, "CheckBonjour: retry counter = {0}", counter));

                    counter++;
                    bContinue = false;
                    try
                    {
                        using (ServiceController sc = new ServiceController(BONJOUR_SERVICE_NAME))
                        {
                            session.Log("CheckBonjour: created service controller");

                            session.Log(string.Format(CultureInfo.InvariantCulture, "CheckBonjour: status={0}", sc.Status));

                            if (ServiceControllerStatus.Running == sc.Status)
                            {
                                session.Log("CheckBonjour: returning success");
                                result = ActionResult.Success;
                            }
                            else
                            {
                                using (Record record = new Record(2))
                                {
                                    record[0] = "[1]";
                                    record[1] = "Bonjour is installed but not running. Please restart your machine";
                                    session.Message(InstallMessage.Error, record);
                                }
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        session.Log(string.Format(CultureInfo.InvariantCulture, "Bonjour-service is not installed. Result={0}", (ex.InnerException != null) ? ex.InnerException.ToString() : ex.ToString()));

                        if (counter <= 1) // Retry once.
                        {
                            using (Record record = new Record(2))
                            {
                                record[0] = "[1]";
                                record[1] = "Bonjour is not installed, which is required to run this OCA tool. Please visit https://support.apple.com/downloads/bonjour to download the Bonjour for Windows installer.\n\rOnce downloaded install it and return here to continue";
                                session.Message(InstallMessage.Error, record);
                            }

                            bContinue = true;
                        }
                    }


                }

                session.Log(string.Format(CultureInfo.InvariantCulture, "CheckBonjour: finished. Return result={0}", result));
            }

            return result;
        }
        #endregion
    }
}
