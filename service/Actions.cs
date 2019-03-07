using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace service
{
    public static class Actions
    {
        delegate bool action(string args);

        public static bool Execute(string command, string args)
        {
            var actions = new Dictionary<string, action>
            {
                { "msgbox", Msgbox }
            };

            foreach (var a in actions)
                if (a.Key == command)
                    return a.Value(args);
            return false;
        }

        private static bool Msgbox(string args)
        {
            System.Windows.Forms.MessageBox.Show(args);
            return true;
        }
    }
}
