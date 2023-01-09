using System.ComponentModel.DataAnnotations;

namespace PlantWateringApp;

public class PlantWateringData
{
    [Key] public DateTime DateTime { get; set; }
    public double Humidity { get; set; }
    public double Threshold { get; set; }
}