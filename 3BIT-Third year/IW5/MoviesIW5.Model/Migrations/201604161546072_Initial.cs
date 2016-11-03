namespace MoviesIW5.Model.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class Initial : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.ActorModels",
                c => new
                    {
                        ID = c.Guid(nullable: false),
                        Image = c.String(),
                        FirstName = c.String(),
                        BirthDate = c.DateTime(nullable: false),
                        LastName = c.String(),
                        Description = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.MovieModels",
                c => new
                    {
                        ID = c.Guid(nullable: false),
                        Country = c.String(),
                        Foto = c.String(),
                        Length = c.Int(nullable: false),
                        NameOriginal = c.String(),
                        NameCzech = c.String(),
                        Description = c.String(),
                        Name = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.DirectorModels",
                c => new
                    {
                        ID = c.Guid(nullable: false),
                        Image = c.String(),
                        FirstName = c.String(),
                        BirthDate = c.DateTime(nullable: false),
                        LastName = c.String(),
                        Description = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.GenreModels",
                c => new
                    {
                        ID = c.Guid(nullable: false),
                        Description = c.String(),
                        Name = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.PrizeModels",
                c => new
                    {
                        ID = c.Guid(nullable: false),
                        Year = c.DateTime(nullable: false),
                        MovieId = c.Int(),
                        Description = c.String(),
                        Name = c.String(),
                        Movie_ID = c.Guid(),
                    })
                .PrimaryKey(t => t.ID)
                .ForeignKey("dbo.MovieModels", t => t.Movie_ID)
                .Index(t => t.Movie_ID);
            
            CreateTable(
                "dbo.ReviewModels",
                c => new
                    {
                        ID = c.Guid(nullable: false),
                        Number = c.Int(nullable: false),
                        Text = c.String(),
                        MovieId = c.Int(),
                        UserId = c.Int(),
                        Movie_ID = c.Guid(),
                        User_ID = c.Guid(),
                    })
                .PrimaryKey(t => t.ID)
                .ForeignKey("dbo.MovieModels", t => t.Movie_ID)
                .ForeignKey("dbo.UserModels", t => t.User_ID)
                .Index(t => t.Movie_ID)
                .Index(t => t.User_ID);
            
            CreateTable(
                "dbo.UserModels",
                c => new
                    {
                        ID = c.Guid(nullable: false),
                        Image = c.String(),
                        FirstName = c.String(),
                        BirthDate = c.DateTime(nullable: false),
                        LastName = c.String(),
                        Description = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.MovieModelActorModels",
                c => new
                    {
                        MovieModel_ID = c.Guid(nullable: false),
                        ActorModel_ID = c.Guid(nullable: false),
                    })
                .PrimaryKey(t => new { t.MovieModel_ID, t.ActorModel_ID })
                .ForeignKey("dbo.MovieModels", t => t.MovieModel_ID, cascadeDelete: true)
                .ForeignKey("dbo.ActorModels", t => t.ActorModel_ID, cascadeDelete: true)
                .Index(t => t.MovieModel_ID)
                .Index(t => t.ActorModel_ID);
            
            CreateTable(
                "dbo.DirectorModelMovieModels",
                c => new
                    {
                        DirectorModel_ID = c.Guid(nullable: false),
                        MovieModel_ID = c.Guid(nullable: false),
                    })
                .PrimaryKey(t => new { t.DirectorModel_ID, t.MovieModel_ID })
                .ForeignKey("dbo.DirectorModels", t => t.DirectorModel_ID, cascadeDelete: true)
                .ForeignKey("dbo.MovieModels", t => t.MovieModel_ID, cascadeDelete: true)
                .Index(t => t.DirectorModel_ID)
                .Index(t => t.MovieModel_ID);
            
            CreateTable(
                "dbo.GenreModelMovieModels",
                c => new
                    {
                        GenreModel_ID = c.Guid(nullable: false),
                        MovieModel_ID = c.Guid(nullable: false),
                    })
                .PrimaryKey(t => new { t.GenreModel_ID, t.MovieModel_ID })
                .ForeignKey("dbo.GenreModels", t => t.GenreModel_ID, cascadeDelete: true)
                .ForeignKey("dbo.MovieModels", t => t.MovieModel_ID, cascadeDelete: true)
                .Index(t => t.GenreModel_ID)
                .Index(t => t.MovieModel_ID);
            
        }
        
        public override void Down()
        {
            DropForeignKey("dbo.ReviewModels", "User_ID", "dbo.UserModels");
            DropForeignKey("dbo.ReviewModels", "Movie_ID", "dbo.MovieModels");
            DropForeignKey("dbo.PrizeModels", "Movie_ID", "dbo.MovieModels");
            DropForeignKey("dbo.GenreModelMovieModels", "MovieModel_ID", "dbo.MovieModels");
            DropForeignKey("dbo.GenreModelMovieModels", "GenreModel_ID", "dbo.GenreModels");
            DropForeignKey("dbo.DirectorModelMovieModels", "MovieModel_ID", "dbo.MovieModels");
            DropForeignKey("dbo.DirectorModelMovieModels", "DirectorModel_ID", "dbo.DirectorModels");
            DropForeignKey("dbo.MovieModelActorModels", "ActorModel_ID", "dbo.ActorModels");
            DropForeignKey("dbo.MovieModelActorModels", "MovieModel_ID", "dbo.MovieModels");
            DropIndex("dbo.GenreModelMovieModels", new[] { "MovieModel_ID" });
            DropIndex("dbo.GenreModelMovieModels", new[] { "GenreModel_ID" });
            DropIndex("dbo.DirectorModelMovieModels", new[] { "MovieModel_ID" });
            DropIndex("dbo.DirectorModelMovieModels", new[] { "DirectorModel_ID" });
            DropIndex("dbo.MovieModelActorModels", new[] { "ActorModel_ID" });
            DropIndex("dbo.MovieModelActorModels", new[] { "MovieModel_ID" });
            DropIndex("dbo.ReviewModels", new[] { "User_ID" });
            DropIndex("dbo.ReviewModels", new[] { "Movie_ID" });
            DropIndex("dbo.PrizeModels", new[] { "Movie_ID" });
            DropTable("dbo.GenreModelMovieModels");
            DropTable("dbo.DirectorModelMovieModels");
            DropTable("dbo.MovieModelActorModels");
            DropTable("dbo.UserModels");
            DropTable("dbo.ReviewModels");
            DropTable("dbo.PrizeModels");
            DropTable("dbo.GenreModels");
            DropTable("dbo.DirectorModels");
            DropTable("dbo.MovieModels");
            DropTable("dbo.ActorModels");
        }
    }
}
