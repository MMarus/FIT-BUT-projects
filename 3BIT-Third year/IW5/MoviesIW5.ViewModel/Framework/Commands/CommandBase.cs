using System;
using System.Windows.Input;

namespace MoviesIW5.ViewModel.Framework.Commands
{
    public abstract class CommandBase<TViewModel> : ICommand
    {
        protected TViewModel ViewModel { get; set; }

        public event EventHandler CanExecuteChanged;

        protected CommandBase(TViewModel viewModel)
        {
            this.ViewModel = viewModel;
        }

        public virtual bool CanExecute(object parameter)
        {
            return true;
        }

        public abstract void Execute(object parameter);
    }
}