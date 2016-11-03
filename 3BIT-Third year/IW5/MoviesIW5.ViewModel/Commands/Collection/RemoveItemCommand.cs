using System;
using MoviesIW5.Model.Base.Interface;
using MoviesIW5.ViewModel.Framework.Commands;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.ViewModel.Commands.Collection
{
    public class RemoveCommand<T> : CommandBase<ViewModelCollection<T>>
        where T : class, IModel, new()
    {
        public RemoveCommand(ViewModelCollection<T> viewModelCollection)
            : base(viewModelCollection)
        {
        }

        public override void Execute(object item)
        {
            
            var typeItem = item as T;

            if (typeItem != null)
            {
                this.ViewModel.Items.Remove(typeItem);
                this.ViewModel.Service.Delete(typeItem);
                this.ViewModel.Service.Save();
            }
        }
    }
}