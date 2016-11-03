using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Input;
using MoviesIW5.Model;
using MoviesIW5.Model.Genres;
using MoviesIW5.ViewModel.Annotations;
using MoviesIW5.Model.Movies;
using MoviesIW5.Model.Persons;
using MoviesIW5.Model.Services;
using MoviesIW5.ViewModel.Commands;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.ViewModel.ViewModels
{
    public class MoviesViewModel : ViewModelCollection<MovieModel>
    {
        public MoviesViewModel()
        {
            this.Name = "Movies";
            this.Service = new MovieService();
            base.LoadData();
            this.NewItem = new MovieModel();
        }
    }
}
