/*
  Homework#4

  Add your name here: Jaden Zaleski

  You are free to create as many classes within the Hw4.cs file or across
  multiple files as you need. However, ensure that the Hw4.cs file is the
  only one that contains a Main method. This method should be within a
  class named hw4. This specific setup is crucial because your instructor
  will use the hw4 class to execute and evaluate your work.
  */
// BONUS POINT:
// => Used Pointers from lines 10 to 15 <=
// => Used Pointers from lines 40 to 63 <=


namespace Homework4;


public class Hw4
{
    public static void Main(string[] args)
    {
        // Capture the start time
        // Must be the first line of this method
        DateTime startTime = DateTime.Now; // Do not change
        // ============================
        // Do not add or change anything above, inside the
        // Main method
        // ============================
        const string root = "../../../";
        const string zipcodePath = root + "zipcodes.txt";

        /*
        Part 1:
        CommonCityNames.txt: contains all the common city names that appear in all of the states listed in states.txt.
            i. Each city in the file must be unique, no duplicate names allowed.
            ii. The cities should appear in sorted order.
            iii. The cities should appear one per line.
        */
        const string commonCitiesOutputFile = root + "CommonCityNames.txt";
        const string statesPath = root + "states.txt";
        var part1 = Part1.CreateInstance(zipcodePath, commonCitiesOutputFile, statesPath);
        part1.GenerateOutput();


        /*
        Part 2:
        LatLon.txt: for each zip code listed in zips.txt, there will be a corresponding line of
        output. Each output line will list the zip code’s latitude and longitude.
            i. The latitude and longitude must be separated by a space on each line.
            ii. If a zip code has multiple entries, provide the first one listed in zipcodes.txt.
        */
        const string latLonOutputFile = root + "LatLon.txt";


        /*
        Part 3:
        CityStates.txt: For each city listed in cities.txt, there will be a corresponding line of
        output. Each output line will list the states containing that city name.
            i. The states must be separated by a space.
            ii. The states should appear in sorted order.
            iii. Each state should be listed only once for each city.
        */
        const string cityStatesOutputFile = root + "CityStates.txt";


        // ============================
        // Do not add or change anything below, inside the
        // Main method
        // ============================

        // Capture the end time
        DateTime endTime = DateTime.Now; // Do not change

        // Calculate the elapsed time
        TimeSpan elapsedTime = endTime - startTime; // Do not change

        // Display the elapsed time in milliseconds
        Console.WriteLine($"Elapsed Time: {elapsedTime.TotalMilliseconds} ms");
    }
}