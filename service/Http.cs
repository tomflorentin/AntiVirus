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
            listener.Prefixes.Add("http://127.0.0.1:8081/");

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
                default:
                    return "404";
            }
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
