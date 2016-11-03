using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MoviesIW5.Model.Movies;

namespace MoviesIW5.Model.Persons
{
    public class ActorModel : PersonModel
    {
        public ICollection<MovieModel> Movies { get; }

        public ActorModel(Guid id) : base(id)
        {
            this.Movies = new HashSet<MovieModel>();
        }

        public ActorModel()
        {
            this.Movies = new HashSet<MovieModel>();
        }
    }
}
