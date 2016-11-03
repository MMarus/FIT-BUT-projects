using System.ComponentModel;
using System.Runtime.CompilerServices;
using MoviesIW5.Model.Annotations;
using MoviesIW5.Model.Base.Interface;
using MoviesIW5.Model.Persons;
using MoviesIW5.Model.Services;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.ViewModel.ViewModels
{
    public class ActorsViewModel : ViewModelCollection<ActorModel>
    {
        public ActorsViewModel()
        {
            this.Name = "Actors";
            this.Service = new ActorService();
            base.LoadData();
            this.NewItem = null;
        }
    }
}
