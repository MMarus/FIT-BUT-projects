using System;
using System.Collections.Generic;
using MoviesIW5.Model.Base.Implementation;
using MoviesIW5.Model.Genres;
using MoviesIW5.Model.Persons;
using MoviesIW5.Model.Prizes;
using MoviesIW5.Model.Reviews;

namespace MoviesIW5.Model.Movies
{
    public class MovieModel : BaseModel
    {
        public string Country { get; set; }

        public string Foto { get; set; }

        public int Length { get; set; }

        public string NameOriginal { get; set; }

        public string NameCzech { get; set; }

        public virtual ICollection<ActorModel> Actors { get; set; }

        public virtual ICollection<DirectorModel> Directors { get; set; }

        public virtual ICollection<ReviewModel> Reviews { get; set; }

        public virtual ICollection<GenreModel> Genres { get; set; }

        public virtual ICollection<PrizeModel> Prizes { get; set; }

        public MovieModel() : base()
        {
            this.Reviews = new HashSet<ReviewModel>();
            this.Genres = new HashSet<GenreModel>();
            this.Prizes = new HashSet<PrizeModel>();
            this.Directors = new HashSet<DirectorModel>();
            this.Actors = new HashSet<ActorModel>();
        }
    }
}