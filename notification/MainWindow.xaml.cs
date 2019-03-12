using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace notification
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            if (Environment.GetCommandLineArgs().Length != 3)
                Environment.Exit(0); // Error

            InitializeComponent();
            action.Content += Environment.GetCommandLineArgs()[1];
            path.Content += Environment.GetCommandLineArgs()[2];
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Environment.Exit(1); // Allow this action
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            Environment.Exit(2); // Deny this action

        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            Environment.Exit(3); // Kill the process

        }
    }
}
