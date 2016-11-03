using System;
using System.Collections.Generic;
using MoviesIW5.Model.Reviews;

namespace MoviesIW5.Model.Persons
{
    public class UserModel : PersonModel
    {
        public virtual ICollection<ReviewModel> Reviews { get; set; }

        public UserModel(Guid id) : base(id)
        {
            Reviews = new HashSet<ReviewModel>();
        }

        public UserModel()
        {
            Reviews = new HashSet<ReviewModel>();
        }
    }

}