using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceProcess;
using System.Diagnostics;
using System.Text;
using System.Threading.Tasks;

namespace service
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            if (System.Diagnostics.Debugger.IsAttached)
            {
                Service myService = new Service();
                myService.OnDebug();
                System.Threading.Thread.Sleep(System.Threading.Timeout.Infinite);
            }

            ServiceBase[] ServicesToRun;
            ServicesToRun = new ServiceBase[]
            {
                new Service()
            };
            ServiceBase.Run(ServicesToRun);
        }
    }
}
