using System;
using System.Collections.Generic;
using MoviesIW5.Model.Base.Implementation;
using MoviesIW5.Model.Movies;

namespace MoviesIW5.Model.Genres
{
    public class GenreModel : BaseModel
    {
        public virtual ICollection<MovieModel> Movies { get; set; }

        public GenreModel() : base()
        {
            this.Movies = new HashSet<MovieModel>();
        }
    }
}
