using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace service
{
    class Http
    {
        private HttpListener listener;


        public Http()
        {
            listener = new HttpListener();
            listener.Start();
            listener.Prefixes.Add("http://127.0.0.1:6890/");

            listener.BeginGetContext(new AsyncCallback(Callback), listener);
        }

        private string Handler(string request, string body)
        {
            switch (request)
            {
                case "/blbl":
                    return "BLLB";
                case "/disp":
                    return "You told me : " + body;
                case "/":
                    return "OK";
                case "/json":
                    return JsonConvert.SerializeObject(new { a = 1, b = "blbl" });
                case "/getInfo":
                    return getInfo(request, body);
                default:
                    return "404";
            }
        }

        private struct info
        {
            public string web;
            public string exploit;
            public string malware;
            public string last;
            public string next;
            public string update;
            public string item;
            public string detection;
            public string timeDetection;
        };
        private string getInfo(string request, string body)
        {

            info stru = new info();
            Microsoft.Win32.RegistryKey key;

            using  (key = Microsoft.Win32.Registry.LocalMachine.OpenSubKey(@"SOFTWARE\WOW6432Node\AlphaDefender")) {
                stru.web = key.GetValue("webProtection") as string;
                stru.exploit = key.GetValue("exploitProtection") as string;
                stru.malware = key.GetValue("malwareProtection") as string;
                stru.last = key.GetValue("lastScan") as string;
                stru.next = key.GetValue("nextScan") as string;
                stru.update = key.GetValue("update") as string;
                stru.item = key.GetValue("itemScanned") as string;
                stru.detection = key.GetValue("scanDetection") as string;
                stru.timeDetection = key.GetValue("runpeDetection") as string;
            }
            return JsonConvert.SerializeObject(stru);       
        }

        private void Callback(IAsyncResult result)
        {
            HttpListener listener = (HttpListener)result.AsyncState;
            HttpListenerContext context = listener.EndGetContext(result);
            HttpListenerRequest request = context.Request;
            HttpListenerResponse response = context.Response;
            byte[] buffer = System.Text.Encoding.UTF8.GetBytes(Handler(request.RawUrl, new StreamReader(request.InputStream).ReadToEnd()));
            response.ContentLength64 = buffer.Length;
            System.IO.Stream output = response.OutputStream;
            output.Write(buffer, 0, buffer.Length);
            output.Close();
            listener.BeginGetContext(new AsyncCallback(Callback), listener);
        }
    }
}
