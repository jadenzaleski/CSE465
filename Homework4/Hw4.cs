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

            string zipcodePath = "../../../zipcodes.txt";
            List<ZipCodeRecord> zips = ZipCodeDataImporter.ImportData(zipcodePath);


            Console.WriteLine("Displaying the first 10 records:");
            for (int i = 0; i < Math.Min(10, zips.Count); i++)
            {
                Console.WriteLine($"Record {i + 1}:");
                Console.WriteLine($"Zipcode: {zips[i].Zipcode}");
                Console.WriteLine($"City: {zips[i].City}");
                Console.WriteLine($"State: {zips[i].State}");
                Console.WriteLine($"Latitude: {zips[i].Lat}");
                Console.WriteLine($"Longitude: {zips[i].Long}");
                Console.WriteLine();
            }






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