using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace PlantWateringApp.Migrations
{
    /// <inheritdoc />
    public partial class ChangePLantWateringData2 : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropPrimaryKey(
                name: "PK_HumidityThresholds",
                table: "HumidityThresholds");

            migrationBuilder.RenameTable(
                name: "HumidityThresholds",
                newName: "PlantWateringData");

            migrationBuilder.AddPrimaryKey(
                name: "PK_PlantWateringData",
                table: "PlantWateringData",
                column: "DateTime");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropPrimaryKey(
                name: "PK_PlantWateringData",
                table: "PlantWateringData");

            migrationBuilder.RenameTable(
                name: "PlantWateringData",
                newName: "HumidityThresholds");

            migrationBuilder.AddPrimaryKey(
                name: "PK_HumidityThresholds",
                table: "HumidityThresholds",
                column: "DateTime");
        }
    }
}
