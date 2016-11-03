using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using MoviesIW5.Model.Annotations;
using MoviesIW5.Model.Base.Interface;

namespace MoviesIW5.Model.Base.Implementation
{
    public abstract class Model : INotifyPropertyChanged, IModel
    {
        public Guid ID { get; private set; }

        protected Model()
        {
            this.ID = Guid.NewGuid();
        }

        protected Model(Guid id)
        {
            this.ID = id;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}