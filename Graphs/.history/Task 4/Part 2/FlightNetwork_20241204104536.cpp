#include "FlightNetwork.h"

/***************************************** FLIGHT CLASS *****************************************/
Flight::Flight() {
    // Initialize all attributes to default values
    flightNumber = "";
    departureAirport = nullptr;
    destinationAirport = nullptr;
    distance = 0;
    cost = 0;
    status = SCHEDULED;
}

Flight::Flight(string flightNumber, shared_ptr<Airport> departureAirport, shared_ptr<Airport> destinationAirport, int distance, int cost, FlightStatus status) {
    // Initialize attributes to provided values
    this->flightNumber = flightNumber;
    this->departureAirport = departureAirport;
    this->destinationAirport = destinationAirport;
    this->distance = distance;
    this->cost = cost;
    this->status = status;
}

shared_ptr<Airport> Flight::getDepartureAirport() {
    return departureAirport;
}

shared_ptr<Airport> Flight::getDestinationAirport() {
    return destinationAirport;
}

string Flight::getFlightNumber() {
    return flightNumber;
}

int Flight::getCost() {
    return cost;
}

int Flight::getDistance() {
    return distance;
}

FlightStatus Flight::getStatus() {
    return status;
}

void Flight::setStatus(FlightStatus status) {
    this->status = status;
}

/***************************************** AIRPORT CLASS *****************************************/
Airport::Airport() {
    name = "";
    city = "";
    country = "";
}

Airport::Airport(string name, string city, string country) {
    this->name = name;
    this->city = city;
    this->country = country;
}

string Airport::getName() {
    return name;
}

string Airport::getCity() {
    return city;
}

string Airport::getCountry() {
    return country;
}

void Airport::addDepartureFlight(string flightNumber, shared_ptr<Airport> destination, int cost, int distance, FlightStatus status) {
    auto flight = make_shared<Flight>(flightNumber, shared_from_this(), destination, distance, cost, status);
    addDepartureFlight(flight);
}

void Airport::addDepartureFlight(shared_ptr<Flight> flight) {
    departureFlights_distance.addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getDistance());
    departureFlights_cost.addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getCost());
}

void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    auto flight = make_shared<Flight>(flightNumber, source, shared_from_this(), distance, cost, status);
    addArrivalFlight(flight);
}

void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    arrivalFlights_distance.addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getDistance());
    arrivalFlights_cost.addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getCost());
}

void Airport::addFlight(shared_ptr<Flight> flight) {
    addDepartureFlight(flight);
    addArrivalFlight(flight);
}

void Airport::TakeOff(string flightNumber) {
    auto flight = getFlight(flightNumber);
    if (flight) {
        flight->setStatus(TAKEN_OFF);
    }
}

void Airport::Land(string flightNumber) {
    auto flight = getFlight(flightNumber);
    if (flight) {
        flight->setStatus(LANDED);
    }
}

bool Airport::hasFlight(string flightNumber) {
    return getFlight(flightNumber) != nullptr;
}

shared_ptr<Flight> Airport::getFlight(string flightNumber) {
    for (auto flight : getAllFlights()) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    return nullptr;
}

vector<shared_ptr<Flight>> Airport::getAllFlights() {
    vector<shared_ptr<Flight>> flights;
    // Combine flights from both departure and arrival graphs
    auto departureEdges = departureFlights_distance.getEdges(shared_from_this());
    auto arrivalEdges = arrivalFlights_distance.getEdges(shared_from_this());
    for (auto edge : departureEdges) {
        flights.push_back(edge.getFlight());
    }
    for (auto edge : arrivalEdges) {
        flights.push_back(edge.getFlight());
    }
    return flights;
}

/************************************** FLIGHTNETWORK CLASS *****************************************/
FlightNetwork::FlightNetwork() {
    // Default constructor
}

void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    if (!hasAirport(airport->getName())) {
        airports.push_back(airport);
    }
}

void FlightNetwork::addFlight(shared_ptr<Flight> flight) {
    auto departure = flight->getDepartureAirport();
    auto destination = flight->getDestinationAirport();

    if (departure && destination && hasAirport(departure->getName()) && hasAirport(destination->getName())) {
        departure->addDepartureFlight(flight);
        destination->addArrivalFlight(flight);
    }
}

bool FlightNetwork::hasAirport(string name) {
    for (auto airport : airports) {
        if (airport->getName() == name) {
            return true;
        }
    }
    return false;
}

bool FlightNetwork::hasFlight(string flightNumber) {
    for (auto flight : getAllFlights()) {
        if (flight->getFlightNumber() == flightNumber) {
            return true;
        }
    }
    return false;
}

shared_ptr<Airport> FlightNetwork::getAirport(string name) {
    for (auto airport : airports) {
        if (airport->getName() == name) {
            return airport;
        }
    }
    return nullptr;
}

shared_ptr<Flight> FlightNetwork::getFlight(string flightNumber) {
    for (auto flight : getAllFlights()) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    return nullptr;
}

// Implement the remaining functions similarly based on the given requirements.
