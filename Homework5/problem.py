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
        # List Comprehension
        states = [state.strip() for state in open(self.infile).read().splitlines() if state.strip()]
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
        # Create a set of unique records
        unique_records = set(self.records)
        # Extract latitudes and longitudes for zip codes
        result_strings = []
        zips = filter(lambda z: z.isdigit() and z != "", open(self.infile).read().splitlines())
        for zip_code in zips:
            matching_record = next((record for record in unique_records if record.zipcode == zip_code), None)
            if matching_record:
                result_strings.append(f"{matching_record.lat} {matching_record.lng}")
        # Write the final list to the output file
        with open(self.outfile, 'w') as writer:
            writer.write("\n".join(result_strings))


'''
CityStates.txt: For each city listed in cities.txt, there will be a corresponding line of
output. Each output line will list the states containing that city name.
i. The states must be separated by a space.
ii. The states should appear in sorted order.
iii. Each state should be listed only once for each city.
'''


class Part3(Problem):
    def generate(self):
        # Read the list of cities from the infile and convert them to uppercase
        cities = map(str.upper, (city.strip() for city in open(self.infile).read().splitlines() if city.strip()))
        # Create a dictionary to store the states containing each city
        city_states = {}
        # Populate the dictionary by iterating over each city
        for city in cities:
            states_containing_city = set()
            # Check each record for the current city
            for record in self.records:
                if record.city == city:
                    states_containing_city.add(record.state)
            # Add the city and its states to the dictionary
            city_states[city] = states_containing_city
        # Write the final list to the output file
        with open(self.outfile, "w") as writer:
            for city, states in city_states.items():
                writer.write(" ".join(states) + "\n")
