namespace Homework4;

public class ZipCodeRecord
{
    public int RecordNumber { get; set; }
    public string Zipcode { get; set; }
    public string ZipCodeType { get; set; }
    public string City { get; set; }
    public string State { get; set; }
    public string LocationType { get; set; }
    public double? Lat { get; set; }
    public double? Long { get; set; }
    public double XAxis { get; set; }
    public double Yaxis { get; set; }
    public double ZAxis { get; set; }
    public string WorldRegion { get; set; }
    public string Country { get; set; }
    public string LocationText { get; set; }
    public string Location { get; set; }
    public bool Decommissioned { get; set; }
    public int? TaxReturnsFiled { get; set; }
    public int? EstimatedPopulation { get; set; }
    public int? TotalWages { get; set; }
    public string? Notes { get; set; }
}

public static class ZipCodeDataImporter
{
    public static List<ZipCodeRecord> ImportData(string filePath)
    {
        var records = new List<ZipCodeRecord>();

        using (StreamReader reader = new StreamReader(filePath))
        {
            // remove the first one
            reader.ReadLine();
            while (reader.ReadLine() is { } line)
            {
                string[] parts = line.Split('\t');
                ZipCodeRecord record = new ZipCodeRecord();
                record.RecordNumber = int.Parse(parts[0].Replace("−", "-"));
                record.Zipcode = parts[1];
                record.ZipCodeType = parts[2];
                record.City = parts[3];
                record.State = parts[4];
                record.LocationType = parts[5];
                if (parts[6] != "")
                    record.Lat = double.Parse(parts[6].Replace("−", "-"));
                if (parts[7] != "")
                    record.Long = double.Parse(parts[7].Replace("−", "-"));
                record.XAxis = double.Parse(parts[8].Replace("−", "-"));
                record.Yaxis = double.Parse(parts[9].Replace("−", "-"));
                record.ZAxis = double.Parse(parts[10].Replace("−", "-"));
                record.WorldRegion = parts[11];
                record.Country = parts[12];
                record.LocationText = parts[13];
                record.Location = parts[14];
                record.Decommissioned = parts[15] == "TRUE";
                record.TaxReturnsFiled = parts.Length > 16 && parts[16] != "" ? int.Parse(parts[16]) : null;
                record.EstimatedPopulation = parts.Length > 17 && parts[17] != "" ? int.Parse(parts[17]) : null;
                record.TotalWages = parts.Length > 18 && parts[18] != "" ? int.Parse(parts[18]) : null;
                record.Notes = parts.Length > 19 ? parts[19] : null;

                records.Add(record);
            }
        }

        return records;
    }
}