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

public class DataReader
{
    private List<ZipCodeRecord> _records;
    private readonly string _outputFile;

    protected DataReader(string zipcodePath, string outputFile)
    {
        _records = ImportData(zipcodePath);
        _outputFile = outputFile;
    }

    private static List<ZipCodeRecord> ImportData(string filePath)
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
        Console.WriteLine("[+] Imported " + records.Count + " records.");
        return records;
    }

    // Accessor methods for protected fields
    protected List<ZipCodeRecord> GetRecords()
    {
        return _records;
    }

    protected string GetOutputFile()
    {
        return _outputFile;
    }
}

public class Part1 : DataReader
{
    private readonly string _statesPath;

    private List<string> states;
    // Constructor
    private Part1(string zipcodePath, string outputFile, string statesPath) : base(zipcodePath, outputFile)
    {
        _statesPath = statesPath;
        states = GenerateStatesList(statesPath);

    }

    // Factory method
    public new static Part1 CreateInstance(string zipcodePath, string outputFile, string statesPath)
    {
        return new Part1(zipcodePath, outputFile, statesPath);
    }

    private List<string> GenerateStatesList(string statesPath)
    {
        List<string> result = new List<string>();
        using (StreamReader reader = new StreamReader(statesPath))
        {
            while (reader.ReadLine() is { } line)
            {
                line = line.ToUpper().Replace(" ", "");
                result.Add(line);
            }
        }

        Console.WriteLine("[+] Part1: Imported " + result.Count + " states.");
        return result;
    }

    public void GenerateOutput()
    {
        List<string> cities = new List<string>();
        var r = GetRecords();
        // get all the cities that match the requirement
        foreach (var state in states)
        {


        }

        Console.WriteLine("[+] Part1: Generating output to " + GetOutputFile() + " for part 1.");


    }
}
