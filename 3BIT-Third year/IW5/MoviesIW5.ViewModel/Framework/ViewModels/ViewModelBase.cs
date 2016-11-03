using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows.Input;
using MoviesIW5.Model.Annotations;
using MoviesIW5.Model;
using MoviesIW5.Model.Base.Interface;
using MoviesIW5.Model.Services;

namespace MoviesIW5.ViewModel.Framework.ViewModels
{
    public abstract class ViewModelBase<T> : INotifyPropertyChanged, IDisposable
        where T : class, IModel
    {
        private bool _disposed;

        private string _name;

        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                if (value == _name)
                {
                    return;
                }
                _name = value;
                OnPropertyChanged();
            }
        }

        public Repository<MoviesDbContext, T> Service { get; protected set; }

        public event PropertyChangedEventHandler PropertyChanged;

        ~ViewModelBase()
        {
            this.Dispose(false);
        }

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        public abstract void LoadData();

        public void SaveData()
        {
            this.Service.Save();
        }

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            /*if (this.Service != null)
            {
                this.Service.Save();
            }*/
            var handler = this.PropertyChanged;
            handler?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void Dispose(bool disposing)
        {
            if (!this._disposed)
            {
                if (disposing)
                {
                    this.Service.Dispose();
                }
                this._disposed = true;
            }
        }

        public virtual void OnProgramShutdownStarted(object sender, EventArgs e)
        {
            this.Service.Save();
            this.Dispose();
        }
    }
}