﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace service
{
    public partial class Service : ServiceBase
    {
        public Service()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            Debug.WriteLine("STARTING ALPHA DEFENDER WINDOWS SERVICE");

            var http = new Http();
            var server = new Server();

            System.Timers.Timer serverClock = new System.Timers.Timer
            {
                Interval = 500
            };
            serverClock.Elapsed += new System.Timers.ElapsedEventHandler(server.Refresh);
            serverClock.Start();
        }

        protected override void OnStop()
        {
        }

        public void OnDebug()
        {
            OnStart(null);
        }
    }
}
