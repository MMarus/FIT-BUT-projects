using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.Entity;
using MoviesIW5.Model.Genres;
using MoviesIW5.Model.Movies;
using MoviesIW5.Model.Persons;
using MoviesIW5.Model.Prizes;
using MoviesIW5.Model.Reviews;

namespace MoviesIW5.Model
{
    public class MoviesDbContext : DbContext
    {
        public MoviesDbContext(): base("MoviesDb") { }

        public DbSet<GenreModel> Genres { get; set; }

        public DbSet<MovieModel> Movies { get; set; }

        public DbSet<ActorModel> Actors { get; set; }

        public DbSet<DirectorModel> Directors { get; set; }

        public DbSet<UserModel> Users { get; set; }

        public DbSet<PrizeModel> Prizes { get; set; }

        public DbSet<ReviewModel> Reviews { get; set; }

        /*public List<MovieModel> getMovies()
        {
            return this.Movies.Load();
        }*/
    }
}
