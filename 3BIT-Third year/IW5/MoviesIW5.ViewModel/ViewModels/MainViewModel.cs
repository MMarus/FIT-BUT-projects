using System;
using MoviesIW5.Model.Base.Interface;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.ViewModel.ViewModels
{
    public class MainViewModel : ViewModelBase<IModel>
    {
        public ActorsViewModel ActorsViewModel { get; set; }
        public MoviesViewModel MoviesViewModel { get; set; }
        public ReviewsViewModel ReviewsViewModel { get; set; }

        public int actualView { get; set; }

        public MainViewModel()
        {
            this.ActorsViewModel = new ActorsViewModel();
            this.MoviesViewModel = new MoviesViewModel();
            this.ReviewsViewModel = new ReviewsViewModel();
        }

        public override void LoadData()
        {
            this.ActorsViewModel.LoadData();
            this.MoviesViewModel.LoadData();
            this.ReviewsViewModel.LoadData();
        }

        public override void OnProgramShutdownStarted(object sender, EventArgs e)
        {
            this.ActorsViewModel.SaveData();
            this.ReviewsViewModel.SaveData();
            this.MoviesViewModel.SaveData();

            this.ActorsViewModel.Dispose();
            this.ReviewsViewModel.Dispose();
            this.MoviesViewModel.Dispose();
        }
    }
}
