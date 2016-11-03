using System;
using System.ComponentModel;

namespace MoviesIW5.Model.Base.Interface
{
    public interface IModel : INotifyPropertyChanged
    {
        Guid ID { get; }
    }
}