#include "FlightNetwork.h"

/***************************************** FLIGHT CLASS *****************************************/
Flight::Flight() {
        : flightNumber(""), distance(0), cost(0), status(FlightStatus::SCHEDULED) {}

}

Flight::Flight(string flightNumber, shared_ptr<Airport> departureAirport, shared_ptr<Airport> destinationAirport, int distance, int cost, FlightStatus status) {
    // Parameterized constructor
    // Initialize all attributes to the provided values
    // distance is in KM and cost is in USD
    // status on the start of the flight is "SCHEDULED"

    // Solution:
}

shared_ptr<Airport> Flight::getDepartureAirport() {
    // Getter for Departure Airport - The airport from where the flight is taking off

    // Solution:
}

shared_ptr<Airport> Flight::getDestinationAirport() {
    // Getter for Destination Airport - The airport where the flight is headed

    // Solution:
}

string Flight::getFlightNumber() {
    // Getter for flight number

    // Solution:
}

int Flight::getCost() {
    // Getter for cost of the flight

    // Solution:
}

int Flight::getDistance() {
    // Getter for distance of the flight

    // Solution:
}

FlightStatus Flight::getStatus() {
    // Getter for status of the flight (SCHEDULED, TAKEN_OFF, LANDED)

    // Solution:
}

void Flight::setStatus(FlightStatus status) {
    // Setter for status of the flight (SCHEDULED, TAKEN_OFF, LANDED)

    // Solution:
}

/***************************************** AIRPORT CLASS *****************************************/
Airport::Airport() {
    // Default constructor
    // Initialize all attributes to default values

    // Solution:
}

Airport::Airport(string name, string city, string country) {
    // Parameterized constructor
    // Initialize all attributes to the provided values

    // Solution:
}

string Airport::getName() {
    // Getter for name of the airport

    // Solution:
}

string Airport::getCity() {
    // Getter for city of the airport

    // Solution:
}

string Airport::getCountry() {
    // Getter for country of the airport

    // Solution:
}

void Airport::addDepartureFlight(string flightNumber, shared_ptr<Airport> destination, int cost, int distance, FlightStatus status) {
    // Add a departure flight from this airport to the destination airport
    // The flight is added to the departureFlights_distance and departureFlights_cost graphs

    // Solution:
}

void Airport::addDepartureFlight(shared_ptr<Flight> flight) {
    // Add a departure flight from this airport to the destination airport
    // The flight is added to the departureFlights_distance and departureFlights_cost graphs
    // The flight is added based on the departure and destination airports

    // Solution:
}

void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    // Add an arrival flight from the source airport to this airport
    // The flight is added to the arrivalFlights_distance and arrivalFlights_cost graphs

    // Solution:
}

void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    // Add an arrival flight from the source airport to this airport
    // The flight is added to the arrivalFlights_distance and arrivalFlights_cost graphs
    // The flight is added based on the departure and destination airports

    // Solution:
}

void Airport::addFlight(shared_ptr<Flight> flight) {
    // Add a flight to the departureFlights_distance, departureFlights_cost, arrivalFlights_distance, and arrivalFlights_cost graphs
    // The flight is added based on the departure and destination airports

    // Solution:
}

void Airport::TakeOff(string flightNumber) {
    // Update the status of the flight with the given flightNumber to TAKEN_OFF

    // Solution:
}

void Airport::Land(string flightNumber) {
    // Update the status of the flight with the given flightNumber to LANDED

    // Solution:
}

bool Airport::hasFlight(string flightNumber) {
    // Check if the flight with the given flightNumber exists in the airport

    // Solution:
}

shared_ptr<Flight> Airport::getFlight(string flightNumber) {
    // Get the flight with the given flightNumber

    // Solution:
}

vector<shared_ptr<Flight>> Airport::getAllFlights() {
    // Get all flights in the airport

    // Solution:
}

/************************************** FLIGHTNETWORK CLASS *****************************************/
FlightNetwork::FlightNetwork() {
    // Default constructor
    // Initialize all attributes to default values

    // Solution:
}

void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    // Correctly add the airport

    // Solution:
}

void FlightNetwork::addFlight(shared_ptr<Flight> flight) {
    // Correctly add the flight to the correct airports
    // Make sure to check for Airport existence

    // Solution:
}

bool FlightNetwork::hasAirport(string name) {
    // Check if the airport exists in the network

    // Solution:
}

bool FlightNetwork::hasFlight(string flightNumber) {
    // Check if the flight exists in the network

    // Solution:
}

shared_ptr<Airport> FlightNetwork::getAirport(string name) {
    // Get the airport with the given name

    // Solution:
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