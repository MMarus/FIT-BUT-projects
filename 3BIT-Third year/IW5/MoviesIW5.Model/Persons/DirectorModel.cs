using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MoviesIW5.Model.Movies;

namespace MoviesIW5.Model.Persons
{
    public class DirectorModel : PersonModel
    {
        public ICollection<MovieModel> Movies { get; }

        public DirectorModel(Guid id) : base(id)
        {
            this.Movies = new HashSet<MovieModel>();
        }
        public DirectorModel()
        {
            this.Movies = new HashSet<MovieModel>();
        }
    }
}
