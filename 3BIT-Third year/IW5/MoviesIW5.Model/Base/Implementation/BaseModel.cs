using System;
using MoviesIW5.Model.Base.Interface;

namespace MoviesIW5.Model.Base.Implementation
{
    public class BaseModel : Model, INameModel, IDescriptionModel
    {
        private string _description;

        private string _name;

        public string Description
        {
            get
            {
                return this._description;
            }
            set
            {
                if (this._description != value)
                {
                    this._description = value;
                    this.OnPropertyChanged();
                }
            }
        }

        public virtual string Name
        {
            get
            {
                return this._name;
            }
            set
            {
                if (this._name != value)
                {
                    this._name = value;
                    this.OnPropertyChanged();
                }
            }
        }

        protected BaseModel() { }

        protected BaseModel(Guid id)
            : base(id)
        { }
    }
}