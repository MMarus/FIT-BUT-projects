using MoviesIW5.Model.Base.Interface;
using MoviesIW5.ViewModel.Framework.Commands;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.ViewModel.Commands.Collection
{
    public class SaveItemCommand<T> : CommandBase<ViewModelCollection<T>>
        where T : class, IModel, new()
    {
        public SaveItemCommand(ViewModelCollection<T> viewModelCollection)
            : base(viewModelCollection)
        {
        }

        public override void Execute(object item)
        {
            this.ViewModel.Service.Add(this.ViewModel.NewItem);
            this.ViewModel.Service.Save();
            this.ViewModel.Items.Add(this.ViewModel.NewItem);
            this.ViewModel.NewItem = new T();
        }

    }
}