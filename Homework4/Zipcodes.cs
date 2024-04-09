namespace Homework4;

public class ZipCodeRecord
{
    public int RecordNumber { get; set; }
    public string Zipcode { get; set; }
    public string ZipCodeType { get; set; }
    public string City { get; set; }
    public string State { get; set; }
    public string LocationType { get; set; }
    public double Lat { get; set; }
    public double Long { get; set; }
    public double Xaxis { get; set; }
    public double Yaxis { get; set; }
    public double Zaxis { get; set; }
    public string WorldRegion { get; set; }
    public string Country { get; set; }
    public string LocationText { get; set; }
    public string Location { get; set; }
    public bool Decommissioned { get; set; }
    public int? TaxReturnsFiled { get; set; }
    public int? EstimatedPopulation { get; set; }
    public decimal? TotalWages { get; set; }
    public string Notes { get; set; }
}

public static class ZipCodeDataImporter
{
    public static List<ZipCodeRecord> ImportData(string filePath)
    {
        List<ZipCodeRecord> records = new List<ZipCodeRecord>();

        using (StreamReader reader = new StreamReader(filePath))
        {
            string line;
            while ((line = reader.ReadLine()) != null)
            {
                string[] parts = line.Split('\t');
                ZipCodeRecord record = new ZipCodeRecord();
                Console.WriteLine(record);
                record.RecordNumber = int.Parse(parts[0]);
                // record.Zipcode = parts[1];
                // record.ZipCodeType = parts[2];
                // record.City = parts[3];
                // record.State = parts[4];
                // record.LocationType = parts[5];
                // record.Lat = double.Parse(parts[6]);
                // record.Long = double.Parse(parts[7]);
                // record.Xaxis = double.Parse(parts[8]);
                // record.Yaxis = double.Parse(parts[9]);
                // record.Zaxis = double.Parse(parts[10]);
                // record.WorldRegion = parts[11];
                // record.Country = parts[12];
                // record.LocationText = parts[13];
                // record.Location = parts[14];
                // record.Decommissioned = parts[15] == "TRUE";
                // record.TaxReturnsFiled = !string.IsNullOrWhiteSpace(parts[16]) ? int.Parse(parts[16]) : (int?)null;
                // record.EstimatedPopulation = !string.IsNullOrWhiteSpace(parts[17]) ? int.Parse(parts[17]) : (int?)null;
                // record.TotalWages = !string.IsNullOrWhiteSpace(parts[18]) ? decimal.Parse(parts[18]) : (decimal?)null;
                // record.Notes = parts[19];

                records.Add(record);
            }
        }

        return records;
    }
}