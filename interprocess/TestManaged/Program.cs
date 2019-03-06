using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestManaged
{
    class Program
    {
        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\x64\Debug\interprocess.dll", CharSet=CharSet.Unicode)]
        static extern int StartServer();

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\x64\Debug\interprocess.dll", CharSet = CharSet.Unicode)]
        static extern int ConnectClient(bool isGui);

        [DllImport(@"C:\Users\tomfl\Desktop\AntiVirus\interprocess\interprocess\x64\Debug\interprocess.dll", CharSet = CharSet.Unicode)]
        static extern int Refresh();

        static void Main(string[] args)
        {
            Console.WriteLine(ConnectClient(true));

            while (true)
            {
                System.Threading.Thread.Sleep(1000);
                Refresh();
            }
        }
    }
}
