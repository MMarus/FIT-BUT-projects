using MoviesIW5.Model.Base.Interface;

namespace MoviesIW5.ViewModel.Framework.ViewModels
{
    public abstract class ViewModelDetail<T> : ViewModelBase<T>
       where T : class, IModel, new()
    {
        protected ViewModelDetail(T item)
        {
            this.Item = item;
        }

        public T Item { get; private set; }

        public override void LoadData()
        {
            Item = Service.GetByID(Item.ID);
        }
    }
}