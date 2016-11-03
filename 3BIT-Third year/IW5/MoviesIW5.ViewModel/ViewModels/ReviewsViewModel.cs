using MoviesIW5.Model.Reviews;
using MoviesIW5.Model.Services;
using MoviesIW5.ViewModel.Framework.ViewModels;

namespace MoviesIW5.ViewModel.ViewModels
{
    public class ReviewsViewModel : ViewModelCollection<ReviewModel>
    {
        public ReviewsViewModel()
        {
            this.Name = "Reviews";
            this.Service = new ReviewService();
            base.LoadData();
            this.NewItem = null;
        }
    }
}
