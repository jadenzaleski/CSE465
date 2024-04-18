class Problem:
    def __init__(self, records, infile, outfile):
        self.records = records
        self.infile = infile
        self.outfile = outfile


'''
CommonCityNames.txt: contains all the common city names that appear in all of the
states listed in states.txt.
i. Each city in the file must be unique, no duplicate names allowed.
ii. The cities should appear in sorted order.
iii. The cities should appear one per line.
'''


class Part1(Problem):
    def generate(self):

        states = [state.strip() for state in open(self.infile).read().splitlines() if state.strip()]
        if len(states) < 2:
            print("[+] Part1: There are less than two states. Cannot compare.")
            return

        # Create and populate a dictionary for each state and all its cities
        state_and_cities = {}
        for state in states:
            city_names = []

            for record in self.records:
                if record.state == state:
                    city_names.append(record.city)

            city_names = list(set(city_names))  # Removes duplicates
            state_and_cities[state] = city_names

        # Find all common cities
        # Compare the first state with all the others
        state1_cities = state_and_cities[list(state_and_cities.keys())[0]]
        for state, cities in state_and_cities.items():
            if state != list(state_and_cities.keys())[0]:
                common_cities = [city for city in state1_cities if city in cities]
                state1_cities = common_cities[:]

        # Sort and remove duplicates
        state1_cities = list(set(state1_cities))
        state1_cities.sort()

        # Clear the output file
        with open(self.outfile, "w") as f:
            f.truncate(0)

        # Write the final list to the output file
        with open(self.outfile, "w") as writer:
            for city in state1_cities:
                writer.write(city + "\n")


'''
LatLon.txt: for each zip code listed in zips.txt, there will be a corresponding line of
output. Each output line will list the zip code’s latitude and longitude.
i. The latitude and longitude must be separated by a space on each line.
ii. If a zip code has multiple entries, provide the first one listed in zipcodes.txt.
'''


class Part2(Problem):
    def generate(self):
        unique_records = set()

        for record in self.records:
            unique_records.add(record)

        result_strings = []

        zips = [zip_code for zip_code in open(self.infile).read().splitlines() if zip_code.isdigit() and zip_code != ""]
        # Output unique zip code records
        for zip_code in zips:
            # find each match and add the Lat and Lon to the list the we will write to the file
            for record in unique_records:
                if record.zipcode == zip_code:
                    result_strings.append(str(record.lat) + ' ' + str(record.lng))
                    break

        # Clear the file
        with open(self.outfile, 'w') as file:
            file.write('')

        # write the final list to the output file
        with open(self.outfile, 'w') as writer:
            for item in result_strings:
                writer.write(item + '\n')



'''
CityStates.txt: For each city listed in cities.txt, there will be a corresponding line of
output. Each output line will list the states containing that city name.
i. The states must be separated by a space.
ii. The states should appear in sorted order.
iii. Each state should be listed only once for each city.
'''


class Part3(Problem):
    def read_input(self):
        with open(self.infile, 'r') as f:
            pass

    def write_output(self):
        with open(self.outfile, 'w') as f:
            pass
