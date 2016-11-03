using System.Collections.ObjectModel;
using System.Threading.Tasks;
using System.Windows.Input;
using MoviesIW5.Model.Base.Interface;
using MoviesIW5.ViewModel.Commands.Collection;

namespace MoviesIW5.ViewModel.Framework.ViewModels
{
    public abstract class ViewModelCollection<T> : ViewModelBase<T>
        where T : class, IModel, new()
    {
        private string _status;

        private T _newItem;

        public ICommand SaveNewItem { get; set; }
        public ICommand DiscardNewItem { get; set; }
        public ICommand RemoveItem { get; set; }

        public ObservableCollection<T> Items
        {
            get
            {
                return this.Service.GetObservableCollection();
            }
        }

        public T NewItem
        {
            get
            {
                return _newItem;
            }
            set
            {
                if (Equals(value, _newItem))
                {
                    return;
                }
                _newItem = value;

                OnPropertyChanged();
            }
        }

        public string Status
        {
            get
            {
                return this._status;
            }
            set
            {
                if (this._status != value)
                {
                    this._status = value;
                    this.OnPropertyChanged();
                }
            }
        }

        protected ViewModelCollection()
        {
            this.Status = "Data nebyla načtena";
            this.RemoveItem = new RemoveCommand<T>(this);
            this.SaveNewItem = new SaveItemCommand<T>(this);
            this.DiscardNewItem = new DiscardItemCommand<T>(this);
            this.NewItem = new T();
        }

        public override async void LoadData()
        {
            this.Status = "Načítám";
            await Task.Delay(10);
            this.Service.LoadAll();
            this.Status = "Data načtena";

            OnPropertyChanged("Items");
        }
    }
}