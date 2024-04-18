import time

from problem import Part1, Part2
from record import Record

"""
  Homework#5

  Add your name here: Jaden Zaleski
  
  => I’m competing for BONUS Points <=

  You are free to create as many classes within the hw5.py file or across 
  multiple files as you need. However, ensure that the hw5.py file is the 
  only one that contains a __main__ method. This specific setup is crucial 
  because your instructor will run the hw5.py file to execute and evaluate 
  your work.
"""

if __name__ == "__main__":
    start_time = time.perf_counter()  # Do not remove this line
    '''
    Inisde the __main__, do not add any codes before this line.
    -----------------------------------------------------------
    '''

    records = []

    with open("zipcodes.txt") as fp:
        for line in fp.readlines()[1:]:
            parts = line.strip().split('\t')
            records.append(Record(parts))

    '''
    Part1
    CommonCityNames.txt: contains all the common city names that appear in all of the
    states listed in states.txt.
    i. Each city in the file must be unique, no duplicate names allowed.
    ii. The cities should appear in sorted order.
    iii. The cities should appear one per line.
    '''

    Part1(records, 'states.txt', 'CommonCityNames.txt').generate()

    '''
    Part2
    LatLon.txt: for each zip code listed in zips.txt, there will be a corresponding line of
    output. Each output line will list the zip code’s latitude and longitude.
    i. The latitude and longitude must be separated by a space on each line.
    ii. If a zip code has multiple entries, provide the first one listed in zipcodes.txt.
    '''

    Part2(records, 'zips.txt', 'LatLon.txt').generate()

    '''
    Part3
    CityStates.txt: For each city listed in cities.txt, there will be a corresponding line of
    output. Each output line will list the states containing that city name.
    i. The states must be separated by a space.
    ii. The states should appear in sorted order.
    iii. Each state should be listed only once for each city.
    '''

    '''
    Inside the __main__, do not add any codes after this line.
    ----------------------------------------------------------
    '''
    end_time = time.perf_counter()
    # Calculate the runtime in milliseconds
    runtime_ms = (end_time - start_time) * 1000
    print(f"The runtime of the program is {runtime_ms} milliseconds.")
