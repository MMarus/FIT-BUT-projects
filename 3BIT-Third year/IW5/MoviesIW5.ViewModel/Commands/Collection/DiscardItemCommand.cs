using MoviesIW5.Model.Base.Interface;
using MoviesIW5.ViewModel.Framework.Commands;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.ViewModel.Commands.Collection
{
    public class DiscardItemCommand<T> : CommandBase<ViewModelCollection<T>>
        where T : class, IModel, new()
    {
        public DiscardItemCommand(ViewModelCollection<T> viewModel)
            : base(viewModel)
        {
        }

        public override void Execute(object parameter)
        {
            ViewModel.NewItem = null;
        }
    }
}