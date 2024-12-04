#include "FlightNetwork.h"

/***************************************** FLIGHT CLASS *****************************************/

Flight::Flight() 
    : flightNumber(""), departureAirport(nullptr), destinationAirport(nullptr), 
      distance(0), cost(0), status(FlightStatus::SCHEDULED) {
    // Default constructor: Initializes all attributes to default values
}

Flight::Flight(string flightNumber, shared_ptr<Airport> departureAirport, 
               shared_ptr<Airport> destinationAirport, int distance, int cost, FlightStatus status)
    : flightNumber(flightNumber), departureAirport(departureAirport), 
      destinationAirport(destinationAirport), distance(distance), cost(cost), status(status) {
    // Parameterized constructor: Initializes attributes with provided values
}

shared_ptr<Airport> Flight::getDepartureAirport() {
    return departureAirport; // Returns the departure airport
}

shared_ptr<Airport> Flight::getDestinationAirport() {
    return destinationAirport; // Returns the destination airport
}

string Flight::getFlightNumber() {
    return flightNumber; // Returns the flight number
}

int Flight::getCost() {
    return cost; // Returns the cost of the flight
}

int Flight::getDistance() {
    return distance; // Returns the distance of the flight
}

FlightStatus Flight::getStatus() {
    return status; // Returns the current status of the flight
}

void Flight::setStatus(FlightStatus status) {
    this->status = status; // Sets the flight's status
}

/***************************************** AIRPORT CLASS *****************************************/

Airport::Airport() 
    : name(""), city(""), country(""), 
      departureFlights_distance(nullptr), departureFlights_cost(nullptr), 
      arrivalFlights_distance(nullptr), arrivalFlights_cost(nullptr) {
    // Default constructor: Initializes attributes to default values
}

Airport::Airport(string name, string city, string country)
    : name(name), city(city), country(country), 
      departureFlights_distance(nullptr), departureFlights_cost(nullptr), 
      arrivalFlights_distance(nullptr), arrivalFlights_cost(nullptr) {
    // Parameterized constructor: Initializes attributes with provided values
}

string Airport::getName() {
    return name; // Returns the name of the airport
}

string Airport::getCity() {
    return city; // Returns the city of the airport
}

string Airport::getCountry() {
    return country; // Returns the country of the airport
}

void Airport::addDepartureFlight(string flightNumber, shared_ptr<Airport> destination, 
                                  int cost, int distance, FlightStatus status) {
    auto flight = make_shared<Flight>(flightNumber, shared_from_this(), destination, distance, cost, status);
    addDepartureFlight(flight); // Adds a departure flight
}

void Airport::addDepartureFlight(shared_ptr<Flight> flight) {
    if (departureFlights_distance && departureFlights_cost) {
        departureFlights_distance->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getDistance());
        departureFlights_cost->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getCost());
    }
}

void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, 
                                int cost, int distance, FlightStatus status) {
    auto flight = make_shared<Flight>(flightNumber, source, shared_from_this(), distance, cost, status);
    addArrivalFlight(flight); // Adds an arrival flight
}

void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    if (arrivalFlights_distance && arrivalFlights_cost) {
        arrivalFlights_distance->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getDistance());
        arrivalFlights_cost->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getCost());
    }
}

void Airport::addFlight(shared_ptr<Flight> flight) {
    addDepartureFlight(flight);
    addArrivalFlight(flight); // Adds the flight to both departure and arrival graphs
}

void Airport::TakeOff(string flightNumber) {
    auto flight = getFlight(flightNumber);
    if (flight) flight->setStatus(FlightStatus::TAKEN_OFF); // Updates flight status to TAKEN_OFF
}

void Airport::Land(string flightNumber) {
    auto flight = getFlight(flightNumber);
    if (flight) flight->setStatus(FlightStatus::LANDED); // Updates flight status to LANDED
}

bool Airport::hasFlight(string flightNumber) {
    return getFlight(flightNumber) != nullptr; // Checks if the flight exists
}

shared_ptr<Flight> Airport::getFlight(string flightNumber) {
    for (const auto& flight : getAllFlights()) {
        if (flight->getFlightNumber() == flightNumber) return flight;
    }
    return nullptr; // Returns the flight if found, nullptr otherwise
}

vector<shared_ptr<Flight>> Airport::getAllFlights() {
    vector<shared_ptr<Flight>> flights;
    // Combine departure and arrival flights (implement logic if needed)
    return flights; // Returns all flights
}

/************************************** FLIGHTNETWORK CLASS *****************************************/

FlightNetwork::FlightNetwork() {
    // Default constructor: Initializes network attributes
}

void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    if (!hasAirport(airport->getName())) airports.push_back(airport); // Adds an airport if it doesn't exist
}

void FlightNetwork::addFlight(shared_ptr<Flight> flight) {
    auto departure = flight->getDepartureAirport();
    auto destination = flight->getDestinationAirport();
    if (hasAirport(departure->getName()) && hasAirport(destination->getName())) {
        departure->addDepartureFlight(flight);
        destination->addArrivalFlight(flight);
    }
}

bool FlightNetwork::hasAirport(string name) {
    return getAirport(name) != nullptr; // Checks if the airport exists
}

bool FlightNetwork::hasFlight(string flightNumber) {
    return getFlight(flightNumber) != nullptr; // Checks if the flight exists
}

shared_ptr<Airport> FlightNetwork::getAirport(string name) {
    for (const auto& airport : airports) {
        if (airport->getName() == name) return airport;
    }
    return nullptr; // Returns the airport if found
}

shared_ptr<Flight> FlightNetwork::getFlight(string flightNumber) {
    for (const auto& flight : getAllFlights()) {
        if (flight->getFlightNumber() == flightNumber) return flight;
    }
    return nullptr; // Returns the flight if found
}

// Additional methods for shortest path, cheapest path, and more can be implemented similarly.
