using global::System;
using global::System.Collections.Generic;
using global::System.IO;
using global::System.Linq;


namespace Homework4
{

    // Object for each record in the file
    public class ZipCodeRecord
    {
        public int RecordNumber;
        public string Zipcode;
        public string ZipCodeType;
        public string City;
        public string State;
        public string LocationType;
        public double? Lat;
        public double? Lon;
        public double XAxis;
        public double Yaxis;
        public double ZAxis;
        public string WorldRegion;
        public string Country;
        public string LocationText;
        public string Location;
        // public bool Decommissioned;
        // public int? TaxReturnsFiled;
        // public int? EstimatedPopulation;
        // public int? TotalWages;
        // public string? Notes;

        // AI helped with these overrides
        public static bool operator ==(ZipCodeRecord a, ZipCodeRecord b)
        {
            // If both are null, or both are same instance, return true
            if (ReferenceEquals(a, b))
                return true;

            // If one is null, but not both, return false
            if (a == null || b == null)
                return false;

            // Return true if the zip codes are equal, false otherwise
            return a.Zipcode == b.Zipcode;
        }

        public static bool operator !=(ZipCodeRecord a, ZipCodeRecord b)
        {
            return !(a == b);
        }

        public override bool Equals(object obj)
        {
            if (obj == null || !(obj is ZipCodeRecord))
                return false;

            return this == (ZipCodeRecord)obj;
        }

        public override int GetHashCode()
        {
            return Zipcode.GetHashCode();
        }

    }

    public class DataReader
    {
        private readonly List<ZipCodeRecord> _records;
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
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    var parts = line.Split('\t');
                    var record = new ZipCodeRecord();
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
                    // record.Decommissioned = parts[15] == "TRUE";
                    // record.TaxReturnsFiled = parts.Length > 16 && parts[16] != "" ? int.Parse(parts[16]) : null;
                    // record.EstimatedPopulation = parts.Length > 17 && parts[17] != "" ? int.Parse(parts[17]) : null;
                    // record.TotalWages = parts.Length > 18 && parts[18] != "" ? int.Parse(parts[18]) : null;
                    // record.Notes = parts.Length > 19 ? parts[19] : null;

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
            _states = GenerateStatesList();
        }

        // Factory method
        public static Part1 CreateInstance(string zipcodePath, string outputFile, string statesPath)
        {
            return new Part1(zipcodePath, outputFile, statesPath);
        }

        private List<string> GenerateStatesList()
        {
            var result = new List<string>();
            using (var reader = new StreamReader(_statesPath))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    if (line == "") continue;
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
            for (int index = 1; index < stateAndCities.Count; index++)
            {
                var item = stateAndCities.ElementAt(index);
                var itemValue = item.Value;

                // Create a new list to store common cities
                var commonCities = state1Cities.Where(c => itemValue.Contains(c)).ToList();

                // Update state1Cities with the common cities
                state1Cities = commonCities.ToList();
            }

            // sort
            state1Cities.Sort();
            state1Cities = state1Cities.Distinct().ToList();
            // Clear the file
            File.WriteAllText(GetOutputFile(), string.Empty);
            // write the final list to the output file
            using (var writer = new StreamWriter(GetOutputFile()))
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
            _zips = GenerateZipsList();
            // AI helped with this statement
            // gets the unique records and the first occurrence of each
            _uniqueRecords = GetRecords()
                .GroupBy(record => record.Zipcode)
                .Select(group => group.First())
                .ToList();
        }

        public static Part2 CreateInstance(string zipcodePath, string outputFile, string statesPath)
        {
            return new Part2(zipcodePath, outputFile, statesPath);
        }

        // generate the list of zipcodes that we can access
        private List<string> GenerateZipsList()
        {
            var result = new List<string>();
            using (var reader = new StreamReader(_zipsPath))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    // make sure zipcode is valid
                    if (line != "" && line.All(char.IsDigit))
                        result.Add(line.Replace(" ", ""));
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
                // find each match and add the Lat and Lon to the list the we will write to the file
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

    public class Part3 : DataReader
    {
        private readonly string _citiesPath;
        private readonly List<string> _cities;

        private Part3(string zipcodePath, string outputFile, string citiesPath) : base(zipcodePath, outputFile)
        {
            _citiesPath = citiesPath;
            _cities = GetCities();
        }

        private List<string> GetCities()
        {
            var result = new List<string>();
            using (var reader = new StreamReader(_citiesPath))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    // Make sure line is valid
                    if (line != "")
                        result.Add(line.ToUpper());
                }
            }

            Console.WriteLine("[+] Part3: Imported " + result.Count + " cities.");
            return result;
        }

        public static Part3 CreateInstance(string zipcodePath, string outputFile, string statesPath)
        {
            return new Part3(zipcodePath, outputFile, statesPath);
        }


        public void GenerateOutput()
        {
            // AI helped with the delegate function and placement of items for matrix array
            // delegate to check if a city exists in a state
            Func<string, string, bool> cityExistsInState = (city, state) =>
                GetRecords().Any(record => record.City == city && record.State == state);

            // Extract distinct and sorted states
            List<string> distinctSortedStates = GetRecords()
                .Select(record => record.State)
                .Distinct()
                .OrderBy(state => state)
                .ToList();

            // Matrix array to store the presence of each city in each state
            bool[,] cityStateMatrix = new bool[_cities.Count, distinctSortedStates.Count];

            // Fill the matrix
            for (var cityIndex = 0; cityIndex < _cities.Count; cityIndex++)
            {
                // Iterate through each city in _cities list
                for (var stateIndex = 0; stateIndex < distinctSortedStates.Count; stateIndex++)
                {
                    // Get the city and state corresponding to the current indices
                    var city = _cities[cityIndex];
                    var state = distinctSortedStates[stateIndex];
                    // Check if the city exists in the current state
                    cityStateMatrix[cityIndex, stateIndex] = cityExistsInState(city, state);
                }
            }

            // Clear the file
            File.WriteAllText(GetOutputFile(), string.Empty);
            // Output the city-state matrix
            using (StreamWriter writer = new StreamWriter(GetOutputFile()))
            {
                for (var cityIndex = 0; cityIndex < _cities.Count; cityIndex++)
                {
                    var city = _cities[cityIndex];
                    var line = "";
                    for (var stateIndex = 0; stateIndex < distinctSortedStates.Count; stateIndex++)
                    {
                        var state = distinctSortedStates[stateIndex];
                        var exists = cityStateMatrix[cityIndex, stateIndex];
                        line += exists ? $"{state} " : "";
                    }

                    writer.WriteLine(line);
                }
            }

            Console.WriteLine("[+] Part3: States written to: " + GetOutputFile());
        }
    }
}