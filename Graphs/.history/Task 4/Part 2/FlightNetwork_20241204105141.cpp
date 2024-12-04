#include "FlightNetwork.h"

/***************************************** FLIGHT CLASS *****************************************/

Flight::Flight()
    : flightNumber(""), departureAirport(nullptr), destinationAirport(nullptr), 
      distance(0), cost(0), status(FlightStatus::SCHEDULED) {
    // Default constructor: Initialize attributes to default values
}

Flight::Flight(string flightNumber, shared_ptr<Airport> departureAirport, shared_ptr<Airport> destinationAirport, 
               int distance, int cost, FlightStatus status)
    : flightNumber(flightNumber), departureAirport(departureAirport), 
      destinationAirport(destinationAirport), distance(distance), cost(cost), status(status) {
    // Parameterized constructor: Initialize attributes to provided values
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

Airport::Airport() 
    : name(""), city(""), country(""), arrivalFlights_distance(make_shared<Graph<shared_ptr<Airport>>>()), 
      arrivalFlights_cost(make_shared<Graph<shared_ptr<Airport>>>()), departureFlights_distance(make_shared<Graph<shared_ptr<Airport>>>()), 
      departureFlights_cost(make_shared<Graph<shared_ptr<Airport>>>()) {
    // Default constructor
}

Airport::Airport(string name, string city, string country)
    : name(name), city(city), country(country), arrivalFlights_distance(make_shared<Graph<shared_ptr<Airport>>>()), 
      arrivalFlights_cost(make_shared<Graph<shared_ptr<Airport>>>()), departureFlights_distance(make_shared<Graph<shared_ptr<Airport>>>()), 
      departureFlights_cost(make_shared<Graph<shared_ptr<Airport>>>()) {
    // Parameterized constructor
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
    departureFlights_distance->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getDistance());
    departureFlights_cost->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getCost());
    departureFlights.push_back(flight);
}

void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    auto flight = make_shared<Flight>(flightNumber, source, shared_from_this(), distance, cost, status);
    addArrivalFlight(flight);
}

void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    arrivalFlights_distance->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getDistance());
    arrivalFlights_cost->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getCost());
    arrivalFlights.push_back(flight);
}

void Airport::addFlight(shared_ptr<Flight> flight) {
    addDepartureFlight(flight);
    addArrivalFlight(flight);
}

void Airport::TakeOff(string flightNumber) {
    auto flight = getFlight(flightNumber);
    if (flight) {
        flight->setStatus(FlightStatus::TAKEN_OFF);
    }
}

void Airport::Land(string flightNumber) {
    auto flight = getFlight(flightNumber);
    if (flight) {
        flight->setStatus(FlightStatus::LANDED);
    }
}

bool Airport::hasFlight(string flightNumber) {
    return getFlight(flightNumber) != nullptr;
}

shared_ptr<Flight> Airport::getFlight(string flightNumber) {
    for (const auto& flight : departureFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    for (const auto& flight : arrivalFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    return nullptr;
}

vector<shared_ptr<Flight>> Airport::getAllFlights() {
    vector<shared_ptr<Flight>> allFlights = departureFlights;
    allFlights.insert(allFlights.end(), arrivalFlights.begin(), arrivalFlights.end());
    return allFlights;
}

/************************************** FLIGHTNETWORK CLASS *****************************************/

FlightNetwork::FlightNetwork() {}

void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    if (!hasAirport(airport->getName())) {
        Airports.push_back(airport);
    }
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
    for (const auto& airport : Airports) {
        if (airport->getName() == name) {
            return true;
        }
    }
    return false;
}

bool FlightNetwork::hasFlight(string flightNumber) {
    for (const auto& airport : Airports) {
        if (airport->hasFlight(flightNumber)) {
            return true;
        }
    }
    return false;
}

shared_ptr<Airport> FlightNetwork::getAirport(string name) {
    for (const auto& airport : Airports) {
        if (airport->getName() == name) {
            return airport;
        }
    }
    return nullptr;
}

shared_ptr<Flight> FlightNetwork::getFlight(string flightNumber) {
    for (const auto& airport : Airports) {
        auto flight = airport->getFlight(flightNumber);
        if (flight) {
            return flight;
        }
    }
    return nullptr;
}

// Remaining functions for shortest paths, optimization, etc., to be implemented as per requirements.
