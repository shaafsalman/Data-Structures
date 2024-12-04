#include "../Task 4/Part 2/FlightNetwork.cpp"

#include <iostream>
#include <cassert>

int main() {
    int totalMarks = 0;

    // Create airports once
    shared_ptr<Airport> airport1 = make_shared<Airport>("JFK", "New York", "USA");
    shared_ptr<Airport> airport2 = make_shared<Airport>("LAX", "Los Angeles", "USA");
    shared_ptr<Airport> airport3 = make_shared<Airport>("ORD", "Chicago", "USA");
    shared_ptr<Airport> airport4 = make_shared<Airport>("SFO", "San Francisco", "USA");

    // Test Case 1: Create airports (1 mark)
    try {
        totalMarks += 1;
        std::cout << "Test Case 1 Passed: Create airports" << std::endl;
    } catch (...) {
        std::cout << "Test Case 1 Failed: Create airports" << std::endl;
    }

    // Test Case 2: Check airport names (1 mark)
    try {
        assert(airport1->getName() == "JFK");
        assert(airport2->getName() == "LAX");
        assert(airport3->getName() == "ORD");
        totalMarks += 1;
        std::cout << "Test Case 2 Passed: Check airport names" << std::endl;
    } catch (...) {
        std::cout << "Test Case 2 Failed: Check airport names" << std::endl;
    }

    // Test Case 3: Add airports to flight network (1 marks)
    try {
        FlightNetwork network;
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

    // Test Case 4: Check if non-existent airport returns false (1 mark)
    try {
        FlightNetwork network;
        network.addAirport(airport1);
        assert(!network.hasAirport("SFO"));
        totalMarks += 1;
        std::cout << "Test Case 4 Passed: Check non-existent airport" << std::endl;
    } catch (...) {
        std::cout << "Test Case 4 Failed: Check non-existent airport" << std::endl;
    }

    // Test Case 5: Create a flight (1 mark)
    try {
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        totalMarks += 1;
        std::cout << "Test Case 5 Passed: Create flight" << std::endl;
    } catch (...) {
        std::cout << "Test Case 5 Failed: Create flight" << std::endl;
    }

    // Test Case 6: Add flights to network (2 marks)
    try {
        FlightNetwork network;
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        assert(network.hasFlight("AA100"));
        totalMarks += 2;
        std::cout << "Test Case 6 Passed: Add flights to network" << std::endl;
    } catch (...) {
        std::cout << "Test Case 6 Failed: Add flights to network" << std::endl;
    }

    // Test Case 7: Check if flight is added (1 mark)
    try {
        FlightNetwork network;
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        assert(network.hasFlight("AA100"));
        totalMarks += 1;
        std::cout << "Test Case 7 Passed: Check if flight is added" << std::endl;
    } catch (...) {
        std::cout << "Test Case 7 Failed: Check if flight is added" << std::endl;
    }

    // Test Case 8: Retrieve flight by number (1 mark)
    try {
        FlightNetwork network;
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        shared_ptr<Flight> retrievedFlight = network.getFlight("AA100");
        assert(retrievedFlight->getFlightNumber() == "AA100");
        totalMarks += 1;
        std::cout << "Test Case 8 Passed: Retrieve flight by number" << std::endl;
    } catch (...) {
        std::cout << "Test Case 8 Failed: Retrieve flight by number" << std::endl;
    }

    // Test Case 9: Check for non-existent flight (1 mark)
    try {
        FlightNetwork network;
        assert(network.getFlight("AA999") == nullptr);
        totalMarks += 1;
        std::cout << "Test Case 9 Passed: Check for non-existent flight" << std::endl;
    } catch (...) {
        std::cout << "Test Case 9 Failed: Check for non-existent flight" << std::endl;
    }

    // Test Case 10: Retrieve airport details from a flight's departure airport (1 mark)
    try {
        FlightNetwork network;
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        assert(flight1->getDepartureAirport()->getName() == "JFK");
        totalMarks += 1;
        std::cout << "Test Case 10 Passed: Retrieve airport details from a flight's departure airport" << std::endl;
    } catch (...) {
        std::cout << "Test Case 10 Failed: Retrieve airport details from a flight's departure airport" << std::endl;
    }

    // Test Case 11: Flight status changes correctly (TAKEN_OFF) (1 mark)
    try {
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        
        airport1->addDepartureFlight(flight1); 
        std::cout << "before  " <<flight1->getStatus() << std::endl;
        airport1->TakeOff("AA100");
        std::cout << "After  " <<flight1->getStatus() << std::endl;

        assert(flight1->getStatus() == FlightStatus::TAKEN_OFF);
        totalMarks += 1;
        std::cout << "Test Case 11 Passed: Flight status changes correctly (TAKEN_OFF)" << std::endl;
    } catch (...) {
        std::cout << "Test Case 11 Failed: Flight status changes correctly (TAKEN_OFF)" << std::endl;
    }

    // Test Case 12: Flight status changes correctly (LANDED) (1 mark)
    try {
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        airport2->Land("AA100");
        assert(flight1->getStatus() == FlightStatus::LANDED);
        totalMarks += 1;
        std::cout << "Test Case 12 Passed: Flight status changes correctly (LANDED)" << std::endl;
    } catch (...) {
        std::cout << "Test Case 12 Failed: Flight status changes correctly (LANDED)" << std::endl;
    }

    // Test Case 13: Get all flights in network (1 mark)
    try {
        FlightNetwork network;
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        network.addFlight(flight2);
        vector<shared_ptr<Flight>> allFlights = network.getAllFlights();
        assert(allFlights.size() == 2);
        totalMarks += 1;
        std::cout << "Test Case 13 Passed: Get all flights in network" << std::endl;
    } catch (...) {
        std::cout << "Test Case 13 Failed: Get all flights in network" << std::endl;
    }

    // Test Case 14: Get all airports in network (1 mark)
    try {
        FlightNetwork network;
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        vector<shared_ptr<Airport>> allAirports = network.getAllAirports();
        assert(allAirports.size() == 3);
        totalMarks += 1;
        std::cout << "Test Case 14 Passed: Get all airports in network" << std::endl;
    } catch (...) {
        std::cout << "Test Case 14 Failed: Get all airports in network" << std::endl;
    }

    // Test Case 15: Check busiest airport (1 mark)
    try {
        FlightNetwork network;
        network.addAirport(airport1);
        shared_ptr<Airport> busiest = network.getBusiestAirport();
        assert(busiest != nullptr);
        totalMarks += 1;
        std::cout << "Test Case 15 Passed: Check busiest airport" << std::endl;
    } catch (...) {
        std::cout << "Test Case 15 Failed: Check busiest airport" << std::endl;
    }

    // Test Case 16: Optimised graph (2 marks)
    try {
        FlightNetwork network;
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        network.addAirport(airport4);
        
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        network.addFlight(flight2);
        
        auto optimisedGraph = network.OptimizedGraph(true);
        assert(optimisedGraph->getAllVertices().size() == 4);
        assert(optimisedGraph->getAllEdges().size() == 2);
        totalMarks += 2;
        std::cout << "Test Case 16 Passed: Optimised graph" << std::endl;
    } catch (...) {
        std::cout << "Test Case 16 Failed: Optimised graph" << std::endl;
    }

    // Test Case 17: Alternate route (3 mark)
    try {
        FlightNetwork network;
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        network.addFlight(flight2);
        
        auto route = network.alternateRouteForFlight(flight1);
        assert(route.size() == 2); // JFK -> LAX -> ORD
        totalMarks += 3;
        std::cout << "Test Case 17 Passed: Alternate route" << std::endl;
    } catch (...) {
        std::cout << "Test Case 17 Failed: Alternate route" << std::endl;
    }

    // Test Case 18: Shortest route (2 mark)
    try {
        FlightNetwork network;
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        network.addFlight(flight2);
        
        auto shortestRoute = network.getShortestPath(airport1, airport3);
        assert(shortestRoute.size() == 2); // JFK -> LAX -> ORD
        totalMarks += 2;
        std::cout << "Test Case 18 Passed: Shortest route" << std::endl;
    } catch (...) {
        std::cout << "Test Case 18 Failed: Shortest route" << std::endl;
    }

    // Test Case 19: Cheapest route (2 mark)
    try {
        FlightNetwork network;
        network.addAirport(airport1);
        network.addAirport(airport2);
        network.addAirport(airport3);
        
        shared_ptr<Flight> flight1 = make_shared<Flight>("AA100", airport1, airport2, 4000, 300, FlightStatus::SCHEDULED);
        shared_ptr<Flight> flight2 = make_shared<Flight>("AA200", airport2, airport3, 3000, 250, FlightStatus::SCHEDULED);
        network.addFlight(flight1);
        network.addFlight(flight2);
        
        auto cheapestRoute = network.getCheapestPath(airport1, airport3);
        assert(cheapestRoute.size() == 2); // JFK -> LAX -> ORD
        totalMarks += 2;
        std::cout << "Test Case 19 Passed: Cheapest route" << std::endl;
    } catch (...) {
        std::cout << "Test Case 19 Failed: Cheapest route" << std::endl;
    }

    // Total Marks Calculation
    std::cout << "Total Marks: " << totalMarks << " out of 25" << std::endl;

    return 0;
}