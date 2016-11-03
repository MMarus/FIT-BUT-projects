using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MoviesIW5.Model.Base.Implementation;

namespace MoviesIW5.Model.Persons
{
    public abstract class PersonModel : BaseModel
    {
        protected PersonModel(Guid id) : base(id) { }

        protected PersonModel() { }
        
        public string Image { get; set; }

        public string FirstName { get; set; }

        public DateTime BirthDate { get; set; }

        public string LastName { get; set; }

        public override string Name
        {
            get { return string.Format("{0} {1}", this.FirstName, this.LastName); }
        }

    }
}
