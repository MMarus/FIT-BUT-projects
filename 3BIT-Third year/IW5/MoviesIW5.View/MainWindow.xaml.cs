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
using MoviesIW5.Model.Base.Interface;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.View
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            
            var dataContextViewModel = DataContext as ViewModelBase<IModel>;
            if (dataContextViewModel != null)
            {
                Dispatcher.ShutdownStarted += dataContextViewModel.OnProgramShutdownStarted;
            }
        }
    }
}
