using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace PlantWateringApp.Migrations
{
    /// <inheritdoc />
    public partial class ChangePLantWateringData : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropPrimaryKey(
                name: "PK_HumidityThresholds",
                table: "HumidityThresholds");

            migrationBuilder.DropColumn(
                name: "Id",
                table: "HumidityThresholds");

            migrationBuilder.RenameColumn(
                name: "Value",
                table: "HumidityThresholds",
                newName: "Threshold");

            migrationBuilder.AddColumn<DateTime>(
                name: "DateTime",
                table: "HumidityThresholds",
                type: "datetime2",
                nullable: false,
                defaultValue: new DateTime(1, 1, 1, 0, 0, 0, 0, DateTimeKind.Unspecified));

            migrationBuilder.AddColumn<double>(
                name: "Humidity",
                table: "HumidityThresholds",
                type: "float",
                nullable: false,
                defaultValue: 0.0);

            migrationBuilder.AddPrimaryKey(
                name: "PK_HumidityThresholds",
                table: "HumidityThresholds",
                column: "DateTime");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropPrimaryKey(
                name: "PK_HumidityThresholds",
                table: "HumidityThresholds");

            migrationBuilder.DropColumn(
                name: "DateTime",
                table: "HumidityThresholds");

            migrationBuilder.DropColumn(
                name: "Humidity",
                table: "HumidityThresholds");

            migrationBuilder.RenameColumn(
                name: "Threshold",
                table: "HumidityThresholds",
                newName: "Value");

            migrationBuilder.AddColumn<Guid>(
                name: "Id",
                table: "HumidityThresholds",
                type: "uniqueidentifier",
                nullable: false,
                defaultValue: new Guid("00000000-0000-0000-0000-000000000000"));

            migrationBuilder.AddPrimaryKey(
                name: "PK_HumidityThresholds",
                table: "HumidityThresholds",
                column: "Id");
        }
    }
}
