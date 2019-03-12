using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace service
{
    class Server
    {
        public Server()
        {
            if (StartServer() != 0)
                throw new Exception("Unable to create server");
            Debug.WriteLine("Server listening !");
        }

        ~Server() 
        {
            StopServer();
        }

        public void Refresh(object sender, System.Timers.ElapsedEventArgs a)
        {
            Refresh();

            int connectedClient = CountClients();

            for (int i = 0; i < connectedClient; i++)
                RefreshClient(i);
        }

        private void RefreshClient(int index)
        {
            IntPtr commandPtr = GetClientOrder(index);
            IntPtr argsPtr;

            if (commandPtr == IntPtr.Zero)
                return;
            string command = Marshal.PtrToStringUni(commandPtr);
            if (command == string.Empty)
                return;
            argsPtr = GetClientArg(index);
            string args = Marshal.PtrToStringUni(argsPtr);

            Actions.Execute(command, args, index);
            FreePtr(commandPtr);
            FreePtr(argsPtr);
        }

        #region imports
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int StartServer(); // Start listening
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void StopServer(); // Stop listening

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int ConnectClient(bool isGui); // Connect to server
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DisconnectClient(); // Disconnect from server

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Refresh(); // Refresh Client / Server

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CountClients(); // Get client count
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendOrder(int pid, string order, string args); // Send order to client

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetClientPid(int index); // Get client PID
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetClientRole(int index); // Get client PID
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetClientPath(int pid); // Get client path
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetClientOrder(int pid); // Get client order (and set to null)
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetClientArg(int pid); // Get client order (and set to null)

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetServiceOrder(); // Get order from service (and set to null)
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetServiceArg(); // Get order from service (and set to null)
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SendServiceOrder(string order, string args);  // Send an order to service

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\Debug\interprocess.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void FreePtr(IntPtr ptr); // Free a pointer to string returner by an other api function
        #endregion
    }
}
