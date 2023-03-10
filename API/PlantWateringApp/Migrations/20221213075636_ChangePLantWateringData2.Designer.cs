// <auto-generated />
using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Migrations;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;
using PlantWateringApp;

#nullable disable

namespace PlantWateringApp.Migrations
{
    [DbContext(typeof(PlantWateringAppDbContext))]
    [Migration("20221213075636_ChangePLantWateringData2")]
    partial class ChangePLantWateringData2
    {
        /// <inheritdoc />
        protected override void BuildTargetModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("ProductVersion", "7.0.0")
                .HasAnnotation("Relational:MaxIdentifierLength", 128);

            SqlServerModelBuilderExtensions.UseIdentityColumns(modelBuilder);

            modelBuilder.Entity("PlantWateringApp.PlantWateringData", b =>
                {
                    b.Property<DateTime>("DateTime")
                        .HasColumnType("datetime2");

                    b.Property<double>("Humidity")
                        .HasColumnType("float");

                    b.Property<double>("Threshold")
                        .HasColumnType("float");

                    b.HasKey("DateTime");

                    b.ToTable("PlantWateringData");
                });
#pragma warning restore 612, 618
        }
    }
}
