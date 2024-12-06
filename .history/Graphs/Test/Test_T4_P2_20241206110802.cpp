#include "../Task 4/Part 2/FlightNetwork.cpp"
#include <iostream>
#include <cassert>

void testCreateAirports(int& totalMarks) {
    try {
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");
        shared_ptr<Airport> airport4 = make_shared<Airport>("SFO", "San Francisco", "USA");
        totalMarks += 1;
        std::cout << "Test Case 1 Passed: Create airports" << std::endl;
    } catch (...) {
        std::cout << "Test Case 1 Failed: Create airports" << std::endl;
    }
}

void testCheckAirportNames(int& totalMarks) {
    try {
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");
        assert(airport1->getName() == "JFK");
        assert(airport2->getName() == "LAX");
        assert(airport3->getName() == "ORD");
        totalMarks += 1;
        std::cout << "Test Case 2 Passed: Check airport names" << std::endl;
    } catch (...) {
        std::cout << "Test Case 2 Failed: Check airport names" << std::endl;
    }
}

void testAddAirportsToNetwork(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");
        shared_ptr<Airport> airport4 = make_shared<Airport>("SFO", "San Francisco", "USA");

        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addAirport(airport4);
        assert(network.hasAirport("JFK"));
        assert(network.hasAirport("LAX"));
        assert(network.hasAirport("ORD"));
        assert(network.hasAirport("SFO"));
        totalMarks += 1;
        std::cout << "Test Case 3 Passed: Add airports to flight network" << std::endl;
    } catch (...) {
        std::cout << "Test Case 3 Failed: Add airports to flight network" << std::endl;
    }
}

void testCheckNonExistentAirport(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        network.addAirport(airport1);
        assert(!network.hasAirport("SFO"));
        totalMarks += 1;
        std::cout << "Test Case 4 Passed: Check non-existent airport" << std::endl;
    } catch (...) {
        std::cout << "Test Case 4 Failed: Check non-existent airport" << std::endl;
    }
}

void testCreateFlight(int& totalMarks) {
    try {
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        totalMarks += 1;
        std::cout << "Test Case 5 Passed: Create flight" << std::endl;
    } catch (...) {
        std::cout << "Test Case 5 Failed: Create flight" << std::endl;
    }
}

void testAddFlightsToNetwork(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addFlight(flight1);
        assert(network.hasFlight("AA100") == true);
        assert(network.hasFlight("AA200") == false);
        totalMarks += 1;
        std::cout << "Test Case 6 Passed: Add flights to network" << std::endl;
    } catch (...) {
        std::cout << "Test Case 6 Failed: Add flights to network" << std::endl;
    }
}

void testRetrieveFlightByNumber(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addFlight(flight1);
        shared_ptr<Flight> retrievedFlight = network.getFlight("AA100");
        assert(retrievedFlight->getFlightNumber() == "AA100");
        totalMarks += 1;
        std::cout << "Test Case 7 Passed: Retrieve flight by number" << std::endl;
    } catch (...) {
        std::cout << "Test Case 7 Failed: Retrieve flight by number" << std::endl;
    }
}

void testCheckForNonExistentFlight(int& totalMarks) {
    try {
        FlightNetwork network;
        assert(network.getFlight("AA999") == nullptr);
        totalMarks += 1;
        std::cout << "Test Case 8 Passed: Check for non-existent flight" << std::endl;
    } catch (...) {
        std::cout << "Test Case 8 Failed: Check for non-existent flight" << std::endl;
    }
}

void testRetrieveAirportDetailsFromFlight(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        assert(flight1->getDepartureAirport()->getName() == "JFK");
        totalMarks += 1;
        std::cout << "Test Case 9 Passed: Retrieve airport details from a flight's departure airport" << std::endl;
    } catch (...) {
        std::cout << "Test Case 9 Failed: Retrieve airport details from a flight's departure airport" << std::endl;
    }
}

void testFlightStatusChangesCorrectlyTakenOff(int& totalMarks) {
    try {
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        airport1->addFlight(flight1);
        airport1->TakeOff("AA100");
        assert(flight1->getStatus() == FlightStatus::TAKEN_OFF);
        totalMarks += 1;
        std::cout << "Test Case 10 Passed: Flight status changes correctly (TAKEN_OFF)" << std::endl;
    } catch (...) {
        std::cout << "Test Case 10 Failed: Flight status changes correctly (TAKEN_OFF)" << std::endl;
    }
}

void testFlightStatusChangesCorrectlyLanded(int& totalMarks) {
    try {
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        airport1->addFlight(flight1);
        airport2->addFlight(flight1);
        airport2->Land("AA100");
        assert(flight1->getStatus() == FlightStatus::LANDED);
        totalMarks += 1;
        std::cout << "Test Case 11 Passed: Flight status changes correctly (LANDED)" << std::endl;
    } catch (...) {
        std::cout << "Test Case 11 Failed: Flight status changes correctly (LANDED)" << std::endl;
    }
}

void testGetAllFlightsInNetwork(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addFlight(flight1);
        network.addFlight(flight2);
        vector<shared_ptr<Flight>> allFlights = network.getAllFlights();
        assert(allFlights.size() == 2);
        totalMarks += 1;
        std::cout << "Test Case 12 Passed: Get all flights in network" << std::endl;
    } catch (...) {
        std::cout << "Test Case 12 Failed: Get all flights in network" << std::endl;
    }
}

void testGetAllAirportsInNetwork(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        vector<shared_ptr<Airport>> allAirports = network.getAllAirports();
        assert(allAirports.size() == 3);
        totalMarks += 1;
        std::cout << "Test Case 13 Passed: Get all airports in network" << std::endl;
    } catch (...) {
        std::cout << "Test Case 13 Failed: Get all airports in network" << std::endl;
    }
}

void testCheckBusiestAirport(int& totalMarks) {
    try {
        FlightNetwork network;
        // Adding 10 airports and 15 flights
        shared_ptr<Airport> airport1 = make_shared<Airport>("Airport1", "City1", "Country1");
        shared_ptr<Airport> airport2 = make_shared<Airport>("Airport2", "City2", "Country2");
        shared_ptr<Airport> airport3 = make_shared<Airport>("Airport3", "City3", "Country3");
        shared_ptr<Airport> airport4 = make_shared<Airport>("Airport4", "City4", "Country4");
        shared_ptr<Airport> airport5 = make_shared<Airport>("Airport5", "City5", "Country5");
        shared_ptr<Airport> airport6 = make_shared<Airport>("Airport6", "City6", "Country6");
        shared_ptr<Airport> airport7 = make_shared<Airport>("Airport7", "City7", "Country7");
        shared_ptr<Airport> airport8 = make_shared<Airport>("Airport8", "City8", "Country8");
        shared_ptr<Airport> airport9 = make_shared<Airport>("Airport9", "City9", "Country9");
        shared_ptr<Airport> airport10 = make_shared<Airport>("Airport10", "City10", "Country10");
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight3 = make_shared<Flight>("AA300", airport3, airport4, 2000, 200, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight4 = make_shared<Flight>("AA400", airport4, airport5, 1000, 150, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight5 = make_shared<Flight>("AA500", airport5, airport6, 500, 100, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight6 = make_shared<Flight>("AA600", airport6, airport7, 400, 90, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight7 = make_shared<Flight>("AA700", airport7, airport8, 300, 80, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight8 = make_shared<Flight>("AA800", airport8, airport9, 200, 70, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight9 = make_shared<Flight>("AA900", airport9, airport10, 100, 60, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight10 = make_shared<Flight>("AA1000", airport1, airport3, 5000, 350, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight11 = make_shared<Flight>("AA1100", airport2, airport4, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight12 = make_shared<Flight>("AA1200", airport3, airport1, 3000, 250, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight13 = make_shared<Flight>("AA1300", airport1, airport6, 2000, 200, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight14 = make_shared<Flight>("AA1400", airport5, airport7, 1000, 150, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight15 = make_shared<Flight>("AA1500", airport9, airport1, 500, 100, FlightStatus::SCHEDULED);
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addAirport(airport4);
        network.addAirport(airport5);
        network.addAirport(airport6);
        network.addAirport(airport7);
        network.addAirport(airport8);
        network.addAirport(airport9);
        network.addAirport(airport10);
        network.addFlight(flight1);
        network.addFlight(flight2);
        network.addFlight(flight3);
        network.addFlight(flight4);
        network.addFlight(flight5);
        network.addFlight(flight6);
        network.addFlight(flight7);
        network.addFlight(flight8);
        network.addFlight(flight9);
        network.addFlight(flight10);
        network.addFlight(flight11);
        network.addFlight(flight12);
        network.addFlight(flight13);
        network.addFlight(flight14);
        network.addFlight(flight15);
        shared_ptr<Airport> busiest = network.getBusiestAirport();
        shared_ptr<Airport> lamest = network.getLamestAirport();
        assert(busiest == airport1);
        assert(lamest == airport10);
        totalMarks += 1;
        std::cout << "Test Case 14 Passed: Check busiest/lamest airport" << std::endl;
    } catch (exception& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Test Case 14 Failed: Check busiest/lamest airport" << std::endl;
    }
}

void testAlternateRoute(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");

        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight3 = make_shared<Flight>("AA300", airport1, airport3, 5000, 350, FlightStatus::SCHEDULED);
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addFlight(flight1);
        network.addFlight(flight2);
        network.addFlight(flight3);
        auto route = network.alternateRouteForFlight(flight3);
                std::cout << "route SIZE" << route.size() << std::endl;

        assert(route.size() == 3); 
        totalMarks += 2;
        std::cout << "Test Case 15 Passed: Alternate route" << std::endl;
    } catch (...) {
        std::cout << "Test Case 15 Failed: Alternate route" << std::endl;
    }
}

void testShortestCheapestRoute(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");

        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight3 = make_shared<Flight>("AA300", airport1, airport3, 8000, 850, FlightStatus::SCHEDULED);
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addFlight(flight1);
        network.addFlight(flight2);
        network.addFlight(flight3);
        auto shortestRoute = network.getShortestPath(airport1, airport3);
        auto cheapestRoute = network.getCheapestPath(airport1, airport3);
        assert(shortestRoute.size() == 3);
        assert(cheapestRoute.size() == 3);
        totalMarks += 1;
        std::cout << "Test Case 16 Passed: Shortest/Cheapest route" << std::endl;
    } catch (...) {
        std::cout << "Test Case 16 Failed: Shortest/Cheapest route" << std::endl;
    }
}

void testGetFlightPlan(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");   
        shared_ptr<Airport> airport4 = make_shared<Airport>("SFO", "San Francisco", "USA");
        shared_ptr<Airport> airport5 = make_shared<Airport>("DFW", "Dallas", "USA");

        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight3 = make_shared<Flight>("AA300", airport3, airport4, 2000, 200, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight4 = make_shared<Flight>("AA400", airport4, airport5, 1000, 150, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight5 = make_shared<Flight>("AA500", airport5, airport1, 5000, 350, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight6 = make_shared<Flight>("AA600", airport1, airport5, 11000, 400, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight7 = make_shared<Flight>("AA700", airport1, airport4, 4000, 350, FlightStatus::SCHEDULED);

        // P1 = JFK -> LAX -> ORD -> SFO -> DFW | D = 10000, C = 900
        // P2 = JFK -> DFW | D = 11000, C = 400
        // P3 = JFK -> SFO -> DFW | D = 5000, C = 500
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addAirport(airport4);
        network.addAirport(airport5);

        network.addFlight(flight1);
        network.addFlight(flight2);
        network.addFlight(flight3);
        network.addFlight(flight4);
        network.addFlight(flight5);
        network.addFlight(flight6);
        network.addFlight(flight7);

        auto flightPlan = network.getFlightPlan(airport1, airport5);
        assert(flightPlan.size() == 3); // JFK -> SFO -> DFW
        totalMarks += 2;
        std::cout << "Test Case 17 Passed: Get flight plan" << std::endl;
    }
    catch (...) {
        std::cout << "Test Case 17 Failed: Get flight plan" << std::endl;
    }
}

void testReachableAirports(int& totalMarks) {
    try {
        FlightNetwork network;
        shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
        shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
        shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");
        shared_ptr<Airport> airport4 = make_shared<Airport>("SFO", "San Francisco", "USA");

        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport4, 3000, 250, FlightStatus::SCHEDULED);

        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addAirport(airport4);
        network.addFlight(flight1);
        network.addFlight(flight2);

        auto reachableFromJFK = network.AirportsReachable(airport1);
        assert(reachableFromJFK.size() == 2);

        totalMarks += 1;
        std::cout << "Test Case 18 Passed: Reachable airports" << std::endl;
    } catch (...) {
        std::cout << "Test Case 18 Failed: Reachable airports" << std::endl;
    }
}

int main() {
    int totalMarks = 0;

    // Run all test cases
    testCreateAirports(totalMarks);
    testCheckAirportNames(totalMarks);
    testAddAirportsToNetwork(totalMarks);
    testCheckNonExistentAirport(totalMarks);
    testCreateFlight(totalMarks);
    testAddFlightsToNetwork(totalMarks);
    testRetrieveFlightByNumber(totalMarks);
    testCheckForNonExistentFlight(totalMarks);
    testRetrieveAirportDetailsFromFlight(totalMarks);
    testFlightStatusChangesCorrectlyTakenOff(totalMarks);
    testFlightStatusChangesCorrectlyLanded(totalMarks);
    testGetAllFlightsInNetwork(totalMarks);
    testGetAllAirportsInNetwork(totalMarks);
    testCheckBusiestAirport(totalMarks);
    testAlternateRoute(totalMarks);
    testShortestCheapestRoute(totalMarks);
    testGetFlightPlan(totalMarks);
    testReachableAirports(totalMarks);

    // Output total marks
    std::cout << "Total Marks: " << totalMarks << " out of 18" << std::endl;
    std::cout << "OptimizedGraph function is not tested in this it will be tested manually (2 Marks)" << std::endl;

    return 0;
}