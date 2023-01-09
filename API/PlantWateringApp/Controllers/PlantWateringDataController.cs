using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace PlantWateringApp.Controllers;

[ApiController]
[Route("[controller]")]
public class PlantWateringDataController : Controller
{
    private readonly PlantWateringAppDbContext _dbContext;

    public PlantWateringDataController(IConfiguration configuration)
    {
        _dbContext = new PlantWateringAppDbContext(configuration);
    }

    [HttpGet(Name = "ReadAll")]
    public async Task<ICollection<PlantWateringData>> GetAll() =>
        await _dbContext.PlantWateringData.ToListAsync();

    [HttpGet("latest", Name = "ReadLatest")]
    public async Task<PlantWateringData> GetLatest() =>
        await _dbContext.PlantWateringData.OrderByDescending(_ => _.DateTime).FirstAsync();

    [HttpGet("chart", Name = "Chart")]
    public async Task<IActionResult> Chart() =>
        View(await _dbContext.PlantWateringData.OrderBy(_ => _.DateTime).ToListAsync());
    
    [HttpPost(Name = "Write")]
    public async Task<int> Post(PlantWateringData model)
    {
        model.DateTime = DateTime.UtcNow;
        _dbContext.Add(model);
        return await _dbContext.SaveChangesAsync();
    }
}