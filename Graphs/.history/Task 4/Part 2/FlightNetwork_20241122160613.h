#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "../../Task 3/Graph_Task_3.cpp"

// Forward declaration
class Airport;

enum class FlightStatus {
    SCHEDULED, // Scheduled - Active flight in both Airports
    TAKEN_OFF, // Taken off - Active flight in arrival Airport
    LANDED // Landed - Not an active flight in either Airport
};

class Flight {
    string flightNumber;
    shared_ptr<Airport> departureAirport;
    shared_ptr<Airport> destinationAirport;
    int distance;
    int cost;
    FlightStatus status;

public:
    Flight();
    Flight(string flightNumber, shared_ptr<Airport> departureAirport, shared_ptr<Airport> destinationAirport, int distance, int cost, FlightStatus status);
    shared_ptr<Airport> getDepartureAirport();
    shared_ptr<Airport> getDestinationAirport();
    string getFlightNumber();
    int getCost();
    int getDistance();
    FlightStatus getStatus();
    void setStatus(FlightStatus status);
    bool operator==(const Flight& flight) const {
        return flightNumber == flight.flightNumber;
    }
    friend ostream& operator<<(ostream& os, const Flight& flight) {
        os << flight.flightNumber;
        return os;
    }
};

class Airport : enable_shared_from_this<Airport>
{
    string name;
    string city;
    string country;
    shared_ptr<Graph<shared_ptr<Airport>>> arrivalFlights_distance; // Airports from which this airport receives flights - edge weight is distance
    shared_ptr<Graph<shared_ptr<Airport>>> arrivalFlights_cost; // Airports from which this airport receives flights - edge weight is cost
    shared_ptr<Graph<shared_ptr<Airport>>> departureFlights_distance; // Airports to which this airport sends flights - edge weight is distance
    shared_ptr<Graph<shared_ptr<Airport>>> departureFlights_cost; // Airports to which this airport sends flights - edge weight is cost
    vector<shared_ptr<Flight>> departureFlights; // Flights departing from this airport
    vector<shared_ptr<Flight>> arrivalFlights; // Flights arriving at this airport

public:
    Airport();
    Airport(string name, string city, string country);
    string getName();
    string getCity();
    string getCountry();
    void addDepartureFlight(string flightNumber, shared_ptr<Airport> destination, int cost, int distance, FlightStatus status);
    void addDepartureFlight(shared_ptr<Flight> flight);
    void addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status);
    void addArrivalFlight(shared_ptr<Flight> flight);
    void addFlight(shared_ptr<Flight> flight);
    void TakeOff(string flightNumber);
    void Land(string flightNumber);
    bool hasFlight(string flightNumber);
    shared_ptr<Flight> getFlight(string flightNumber);
    vector<shared_ptr<Flight>> getAllFlights();
    bool operator==(const Airport& airport) const {
        return name == airport.name;
    }
    friend ostream& operator<<(ostream& os, const Airport& airport) {
        os << airport.name;
        return os;
    }
};

class FlightNetwork {
    Graph<shared_ptr<Airport>> AirportNetwork_distance; // Graph representing the network of airports - edge weight is distance
    Graph<shared_ptr<Airport>> AirportNetwork_cost; // Graph representing the network of airports - edge weight is cost
    vector<shared_ptr<Airport>> Airports; // List of all airports in the network

public:
    FlightNetwork();
    void addAirport(shared_ptr<Airport> airport);
    void addFlight(shared_ptr<Flight> flight);
    bool hasAirport(string name);
    bool hasFlight(string flightNumber);
    shared_ptr<Airport> getAirport(string name);
    shared_ptr<Flight> getFlight(string flightNumber);
    vector<shared_ptr<Airport>> getShortestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination);
    vector<shared_ptr<Airport>> getCheapestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination);
    vector<shared_ptr<Airport>> getFlightPlan(shared_ptr<Airport> source, shared_ptr<Airport> destination);
    vector<shared_ptr<Flight>> getAllFlights();
    vector<shared_ptr<Airport>> getAllAirports();
    shared_ptr<Airport> getBusiestAirport(); // Airport with most flights incoming and outgoing
    shared_ptr<Airport> getLamestAirport(); // Airport with least flights incoming and outgoing
    shared_ptr<Graph<shared_ptr<Airport>>> OptimizedGraph(bool distance);
    vector<shared_ptr<Airport>> alternateRouteForFlight(shared_ptr<Flight> flight); // Alternate routes for a flight
    vector<shared_ptr<Airport>> AirportsReachable(shared_ptr<Airport> airport); // Airports reachable from a given airport
};