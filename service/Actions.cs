using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace service
{
    public static class Actions
    {
        delegate bool action(string args, int index);

        public static bool Execute(string command, string args, int index)
        {
            var actions = new Dictionary<string, action>
            {
                { "msgbox", Msgbox },
                { "confirm", Confirm },
                {"delete", Delete }
            };

            foreach (var a in actions)
                if (a.Key == command)
                    return a.Value(args, index);
            return false;
        }

        private static bool Delete(string args, int index)
        {
            int pid = Server.GetClientPid(index);

            new Task(() => {
                while(System.IO.File.Exists(args))
                {
                    try
                    {
                        System.IO.File.Delete(args);
                    } catch { System.Threading.Thread.Sleep(500); };
                }
            }).Start();

            return true;
        }

        private static bool Msgbox(string args, int index)
        {
            System.Windows.Forms.MessageBox.Show(args);
            return true;
        }

        private static bool Confirm(string args, int index)
        {
            IntPtr pathPtr = Server.GetClientPath(index);
            string path = Marshal.PtrToStringUni(pathPtr);
            
            using (Process notif = new Process())
            {
                notif.StartInfo.FileName = @"C:\Users\tomfl\Desktop\AntiVirus\notification\bin\Debug\notification.exe";
                notif.StartInfo.Arguments = "\"" + args + "\" \"" + path + "\"";
                notif.Start();
                notif.WaitForExit();
                if (notif.ExitCode == 1)
                    Server.SendOrder(index, "allow", "");
                if (notif.ExitCode == 2)
                    Server.SendOrder(index, "block", "");
                if (notif.ExitCode == 3)
                    Server.SendOrder(index, "kill", "");
            }
            Server.FreePtr(pathPtr);
            return true;
        }

    }
}
