using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using System.ComponentModel;

namespace service
{
    class MemoryScan
    {
        [DllImport(@"C:\Users\thoqu\source\repos\Dll1\Debug\Dll1.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void scan(); //Scan
        [DllImport(@"C:\Users\thoqu\source\repos\Dll1\Debug\Dll1.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getSize(); //Size
        [DllImport(@"C:\Users\thoqu\source\repos\Dll1\Debug\Dll1.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int getElemAt(int i); //elem
        public MemoryScan()
        {
            
        }

        public void ScanProcess()
        {
            scan();
        }

        public int getVectorSize()
        {
            return getSize();
        }
        public int getElem(int i)
        {
            return getElemAt(i);
        }
        // Boucler sur tous les process; Open process; Header (struct) (readProcessMemory) lire dans la mémoiré le process, getModuleFileNameEx me donne le vrai path;  
        //getModulelblbl = path du vrai, lire les header du vrai, cmp header ReadProcessMemory
    }
}
