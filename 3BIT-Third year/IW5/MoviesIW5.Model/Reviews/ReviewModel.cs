using System;
using MoviesIW5.Model.Movies;
using MoviesIW5.Model.Base.Implementation;
using MoviesIW5.Model.Persons;

namespace MoviesIW5.Model.Reviews
{
    public class ReviewModel : Base.Implementation.Model
    {
        public int Number { get; set; }

        public string Text { get; set; }

        public int? MovieId { get; set; }

        public virtual MovieModel Movie { get; set; }

        public int? UserId { get; set; }

        public virtual UserModel User { get; set; }

        protected ReviewModel(Guid id) : base(id)
        {
        }

        public ReviewModel()
        {
        }
    }
}
