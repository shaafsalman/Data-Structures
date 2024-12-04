#include "FlightNetwork.h"

/***************************************** FLIGHT CLASS *****************************************/
Flight::Flight() 
    : flightNumber(""), distance(0), cost(0), status(FlightStatus::SCHEDULED) {}


Flight::Flight(string flightNumber, shared_ptr<Airport> departureAirport, shared_ptr<Airport> destinationAirport, int distance, int cost, FlightStatus status) 
    : flightNumber(flightNumber), 
      departureAirport(departureAirport), 
      destinationAirport(destinationAirport), 
      distance(distance), 
      cost(cost), 
      status(status) {}

shared_ptr<Airport> Flight::getDepartureAirport() {
    // Getter for Departure Airport - The airport from where the flight is taking off

    // Solution:
        return departureAirport;

}

shared_ptr<Airport> Flight::getDestinationAirport() {
    // Getter for Destination Airport - The airport where the flight is headed

    // Solution:
        return destinationAirport;

}

string Flight::getFlightNumber() {
    // Getter for flight number

    // Solution:
        return flightNumber;

}

int Flight::getCost() {
    // Getter for cost of the flight

    // Solution:
        return cost;

}

int Flight::getDistance() {
    // Getter for distance of the flight

    // Solution:
        return distance;

}

FlightStatus Flight::getStatus() {
    // Getter for status of the flight (SCHEDULED, TAKEN_OFF, LANDED)

    // Solution:
        return status;

}

void Flight::setStatus(FlightStatus status) {
    // Setter for status of the flight (SCHEDULED, TAKEN_OFF, LANDED)

    // Solution:
        this->status = status;

}

/***************************************** AIRPORT CLASS *****************************************/
Airport::Airport() 
    : name(""), city(""), country("") {
    arrivalFlights_distance = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    arrivalFlights_cost = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    departureFlights_distance = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    departureFlights_cost = make_shared<Graph<shared_ptr<Airport>>>(true, true);
}

Airport::Airport(string name, string city, string country) 
    : name(name), city(city), country(country) {
    arrivalFlights_distance = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    arrivalFlights_cost = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    departureFlights_distance = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    departureFlights_cost = make_shared<Graph<shared_ptr<Airport>>>(true, true);
}

string Airport::getName() {
    // Getter for name of the airport

    // Solution:
        return name;

}

string Airport::getCity() {
    // Getter for city of the airport

    // Solution:
        return city;

}

string Airport::getCountry() {
    // Getter for country of the airport

    // Solution:
        return country;

}







void Airport::addDepartureFlight(string flightNumber, shared_ptr<Airport> destination, int cost, int distance, FlightStatus status) {
    // Create a new flight
    auto flight = make_shared<Flight>(flightNumber, shared_from_this(), destination, distance, cost, status);
    // Add to the list of departure flights
    departureFlights.push_back(flight);
    // Add to departure graphs
    departureFlights_distance->addEdge(shared_from_this(), destination, distance);
    departureFlights_cost->addEdge(shared_from_this(), destination, cost);
}

void Airport::addDepartureFlight(shared_ptr<Flight> flight) {
    // Add to the list of departure flights
    departureFlights.push_back(flight);
    // Add to departure graphs
    departureFlights_distance->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getDistance());
    departureFlights_cost->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getCost());
}

void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    // Create a new flight
    auto flight = make_shared<Flight>(flightNumber, source, shared_from_this(), distance, cost, status);
    // Add to the list of arrival flights
    arrivalFlights.push_back(flight);
    // Add to arrival graphs
    arrivalFlights_distance->addEdge(source, shared_from_this(), distance);
    arrivalFlights_cost->addEdge(source, shared_from_this(), cost);
}

void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    // Add to the list of arrival flights
    arrivalFlights.push_back(flight);
    // Add to arrival graphs
    arrivalFlights_distance->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getDistance());
    arrivalFlights_cost->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getCost());
}







void Airport::addFlight(shared_ptr<Flight> flight) {
    // Add a flight to the departureFlights_distance, departureFlights_cost, arrivalFlights_distance, and arrivalFlights_cost graphs
    // The flight is added based on the departure and destination airports

    // Solution:
     addDepartureFlight(flight);
    addArrivalFlight(flight);
}

void Airport::TakeOff(string flightNumber) {
    // Update the status of the flight with the given flightNumber to TAKEN_OFF

    // Solution:
      auto flight = getFlight(flightNumber);
    if (flight) {
        flight->setStatus(FlightStatus::TAKEN_OFF);
    }
}

void Airport::Land(string flightNumber) {
    // Update the status of the flight with the given flightNumber to LANDED

    // Solution:
     auto flight = getFlight(flightNumber);
    if (flight) {
        flight->setStatus(FlightStatus::LANDED);
    }
}

bool Airport::hasFlight(string flightNumber) {
    // Check if the flight with the given flightNumber exists in the airport

    // Solution:
    for (auto &flight : departureFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return true;
        }
    }
    for (auto &flight : arrivalFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return true;
        }
    }
    return false;
}

shared_ptr<Flight> Airport::getFlight(string flightNumber) {
    // Get the flight with the given flightNumber

    // Solution:
    for (auto &flight : departureFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    for (auto &flight : arrivalFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    return nullptr;
}

vector<shared_ptr<Flight>> Airport::getAllFlights() {
    // Get all flights in the airport

    // Solution:
    vector<shared_ptr<Flight>> allFlights = departureFlights;
    allFlights.insert(allFlights.end(), arrivalFlights.begin(), arrivalFlights.end());
    return allFlights;
}

/************************************** FLIGHTNETWORK CLASS *****************************************/
FlightNetwork::FlightNetwork()
    : AirportNetwork_distance(true, true), AirportNetwork_cost(true, true) {}


void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    // Correctly add the airport

    // Solution:
     if (!hasAirport(airport->getName())) {
        Airports.push_back(airport);
        AirportNetwork_distance.addVertex(airport);
        AirportNetwork_cost.addVertex(airport);
    }
}

void FlightNetwork::addFlight(shared_ptr<Flight> flight) {
    // Correctly add the flight to the correct airports
    // Make sure to check for Airport existence

    // Solution:
    auto departureAirport = flight->getDepartureAirport();
    auto destinationAirport = flight->getDestinationAirport();

    if (!hasAirport(departureAirport->getName()) || !hasAirport(destinationAirport->getName())) {
        return; // Do not add the flight if the airports are not in the network
    }

    departureAirport->addDepartureFlight(flight);
    destinationAirport->addArrivalFlight(flight);

    AirportNetwork_distance.addEdge(departureAirport, destinationAirport, flight->getDistance());
    AirportNetwork_cost.addEdge(departureAirport, destinationAirport, flight->getCost());
}

bool FlightNetwork::hasAirport(string name) {
    // Check if the airport exists in the network

    // Solution:
    for (auto &airport : Airports) {
        if (airport->getName() == name) {
            return true;
        }
    }
    return false;
}

bool FlightNetwork::hasFlight(string flightNumber) {
    // Check if the flight exists in the network

    // Solution:
    for (auto &airport : Airports) {
        if (airport->hasFlight(flightNumber)) {
            return true;
        }
    }
    return false;
}

shared_ptr<Airport> FlightNetwork::getAirport(string name) {
    // Get the airport with the given name

    // Solution:
      for (auto &airport : Airports) {
        if (airport->getName() == name) {
            return airport;
        }
    }
    return nullptr;
}

shared_ptr<Flight> FlightNetwork::getFlight(string flightNumber) {
    // Get the flight with the given flightNumber

    // Solution:
}

vector<shared_ptr<Airport>> FlightNetwork::getShortestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    // Get the shortest path from source to destination based on the given criteria

    // Solution:
}

vector<shared_ptr<Airport>> FlightNetwork::getCheapestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    // Get the cheapest path from source to destination based on the given criteria

    // Solution:
}

vector<shared_ptr<Airport>> FlightNetwork::getFlightPlan(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    // Get the flight plan from source to destination based on the given criteria
    // for example:
    // TOTAL PATHS (P1 for PATH 1): {P1, P2, P3, P4, P5}
    // SHORTEST PATH (SHORTEST FIRST) : {P2, P3, P4, P5, P1}
    // CHEAPEST PATH (CHEAPEST FIRST) : {P1, P4, P5, P3, P2}
    // FLIGHT PLAN = P4
    
    // Solution:
}

vector<shared_ptr<Flight>> FlightNetwork::getAllFlights() {
    // Get all flights in the network - No duplicates

    // Solution:
}

vector<shared_ptr<Airport>> FlightNetwork::getAllAirports() {
    // Get all airports in the network

    // Solution:
}

shared_ptr<Airport> FlightNetwork::getBusiestAirport() {
    // Get the busiest airport in the network

    // Solution:
}

shared_ptr<Airport> FlightNetwork::getLamestAirport() {
    // Get the lameset airport in the network

    // Solution:
}

shared_ptr<Graph<shared_ptr<Airport>>> FlightNetwork::OptimizedGraph(bool distance) {
    // Optimize the graph.
    // Return the optimized graph

    // Solution:
}

vector<shared_ptr<Airport>> FlightNetwork::alternateRouteForFlight(shared_ptr<Flight> flight) {
    // Imagine that the flight can not take the direct route due to some reason
    // Find alternate routes for the flight
    // if cheapest and shortest route are the same then return it otherwise return the one with the least % increase.
    // FORMULA = [(new_distance - old_distance) / old_distance * 100] + [(new_cost - old_cost) / old_cost * 100]
    // Solution:
}

vector<shared_ptr<Airport>> FlightNetwork::AirportsReachable(shared_ptr<Airport> airport) {
    // Get all the airports reachable from the given airport
    // Return a vector of vectors where each vector contains the airports reachable from the given airport

    // Solution:
}