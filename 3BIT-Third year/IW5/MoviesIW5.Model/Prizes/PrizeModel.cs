using System;
using MoviesIW5.Model.Base.Implementation;
using MoviesIW5.Model.Movies;

namespace MoviesIW5.Model.Prizes
{
    public class PrizeModel : BaseModel
    {
        public DateTime Year { get; set; }

        public int? MovieId { get; set; }

        public virtual MovieModel Movie { get; set; }

        protected PrizeModel(Guid id) : base(id)
        {
        }
        protected PrizeModel()
        {
        }
    }
}
