using Microsoft.EntityFrameworkCore;

namespace PlantWateringApp;

public class PlantWateringAppDbContext : DbContext
{
    private readonly IConfiguration _configuration;

    public PlantWateringAppDbContext(IConfiguration configuration)
    {
        _configuration = configuration;
    }

    protected override void OnConfiguring(DbContextOptionsBuilder options)
        => options.UseSqlServer(_configuration.GetConnectionString("DefaultConnection"));

    public DbSet<PlantWateringData> PlantWateringData { get; set; } = null!;
}