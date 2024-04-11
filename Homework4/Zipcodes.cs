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
    public double? Lon { get; set; }
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
                    record.Lon = double.Parse(parts[7].Replace("−", "-"));
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

    private readonly List<string> _states;
    // Constructor
    private Part1(string zipcodePath, string outputFile, string statesPath) : base(zipcodePath, outputFile)
    {
        _statesPath = statesPath;
        _states = GenerateStatesList(statesPath);

    }

    // Factory method
    public new static Part1 CreateInstance(string zipcodePath, string outputFile, string statesPath)
    {
        return new Part1(zipcodePath, outputFile, statesPath);
    }

    private List<string> GenerateStatesList(string statesPath)
    {
        var result = new List<string>();
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
        if (_states.Count < 2)
        {
            Console.WriteLine("[+] Part1: There are less then two states. Cannot compare.");
            return;
        }
        List<string> cities = new List<string>();
        var r = GetRecords();

        // create a populate a dictionary for each state and all its cities
        var stateAndCities = new Dictionary<string, List<string>>();
        foreach (var state in _states)
        {
            var cityNames = new List<string>();

            foreach (var record in r)
            {
                if (record.State == state)
                {
                    cityNames.Add(record.City);
                }
            }
            cityNames = cityNames.Distinct().ToList();
            stateAndCities.Add(state, cityNames);
        }

        // now we need to find all common cities.
        // to do this we can just compare the first state with all the others.

        var state1Cities = stateAndCities.ElementAt(0).Value;
        for (int index = 1; index < stateAndCities.Count; index++) {
            var item = stateAndCities.ElementAt(index);
            var itemKey = item.Key;
            var itemValue = item.Value;

            // Create a new list to store common cities
            List<string> commonCities = new List<string>();

            foreach (var c in state1Cities)
            {
                if (itemValue.Contains(c))
                {
                    string s;
                    commonCities.Add(c);
                }
            }

            // Update state1Cities with the common cities
            state1Cities = commonCities.ToList();
        }

        // sort
        state1Cities.Sort();
        state1Cities = state1Cities.Distinct().ToList();
        // Clear the file
        File.WriteAllText(GetOutputFile(), string.Empty);
        // write the final list to the output file
        using (StreamWriter writer = new StreamWriter(GetOutputFile()))
        {
            foreach (var city in state1Cities)
            {
                writer.WriteLine(city);
            }
        }

        Console.WriteLine("[+] Part1: Common cities written to: " + GetOutputFile());

    }
}

public class Part2 : DataReader
{
    private readonly string _zipsPath;
    private readonly List<string> _zips;
    private readonly List<ZipCodeRecord> _uniqueRecords;
    private Part2(string zipcodePath, string outputFile, string zipsPath) : base(zipcodePath, outputFile)
    {
        _zipsPath = zipsPath;
        _zips = GenerateZipsList(zipsPath);
        _uniqueRecords = GetRecords()
            .GroupBy(record => record.Zipcode)
            .Select(group => group.First())
            .ToList();
    }

    public new static Part2 CreateInstance(string zipcodePath, string outputFile, string statesPath)
    {
        return new Part2(zipcodePath, outputFile, statesPath);
    }

    private List<string> GenerateZipsList(string zipsPath)
    {
        var result = new List<string>();
        using (var reader = new StreamReader(zipsPath))
        {
            while (reader.ReadLine() is { } line)
            {
                line = line.ToUpper().Replace(" ", "");
                result.Add(line);
            }
        }

        Console.WriteLine("[+] Part2: Imported " + result.Count + " zipcodes.");
        return result;
    }

    public void GenerateOutput()
    {
        var resultStrings = new List<string>();
        // Output unique zip code records
        foreach (var zip in _zips)
        {
            foreach (var r in _uniqueRecords.Where(r => r.Zipcode == zip))
            {
                resultStrings.Add(r.Lat + " " + r.Lon);
                break;
            }
        }

        // Clear the file
        File.WriteAllText(GetOutputFile(), string.Empty);
        // write the final list to the output file
        using (StreamWriter writer = new StreamWriter(GetOutputFile()))
        {
            foreach (var x in resultStrings)
            {
                writer.WriteLine(x);
            }
        }

        Console.WriteLine("[+] Part2: Lat and Lon written to: " + GetOutputFile());
    }

}
