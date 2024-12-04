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
      status(status) {

        
        if (departureAirport) {
        departureAirport->addDepartureFlight(shared_ptr<Flight>(this));
    }
      if (destinationAirport) {
        destinationAirport->addArrivalFlight(shared_ptr<Flight>(this));
    }
    
    // // Debugging: Print the constructor parameters to verify their values
    // std::cout << "Creating Flight: " << std::endl;
    // std::cout << "Flight Number: " << flightNumber << std::endl;
    // std::cout << "Departure Airport: " << (departureAirport ? departureAirport->getName() : "null") << std::endl;
    // std::cout << "Destination Airport: " << (destinationAirport ? destinationAirport->getName() : "null") << std::endl;
    // std::cout << "Distance: " << distance << std::endl;
    // std::cout << "Cost: " << cost << std::endl;
    // std::cout << "Status: " << status << std::endl;
}


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
    // std::cout << "Get Status of flight " << flightNumber << " " << this->status << std::endl;

    // Solution:
        return status;

}

void Flight::setStatus(FlightStatus status) {
    // Setter for status of the flight (SCHEDULED, TAKEN_OFF, LANDED)
    // std::cout << "set Status of flight " << flightNumber << " changed to " << status << std::endl;

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
    // Check for invalid parameters
    if (!destination || flightNumber.empty()) {
        return;
    }

    // Create a new flight
    auto flight = make_shared<Flight>(flightNumber, shared_from_this(), destination, distance, cost, status);
    if (!flight) {
        return;
    }

    // Add to the list of departure flights
    departureFlights.push_back(flight);
    departureFlights_distance->addEdge(shared_from_this(), destination, distance);
    departureFlights_cost->addEdge(shared_from_this(), destination, cost);



}

void Airport::addDepartureFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        return;
    }

    // Add to the list of departure flights
    departureFlights.push_back(flight);

   departureFlights_distance->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getDistance());
   departureFlights_cost->addEdge(shared_from_this(), flight->getDestinationAirport(), flight->getCost());


 
}

void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    // Check for invalid parameters
    if (!source || flightNumber.empty()) {
        return;
    }


    // Create a new flight
    auto flight = make_shared<Flight>(flightNumber, source, shared_from_this(), distance, cost, status);
    if (!flight) {
        return;
    }

    // Add to the list of arrival flights
    arrivalFlights.push_back(flight);
    arrivalFlights_distance->addEdge(source, shared_from_this(), distance);
    arrivalFlights_cost->addEdge(source, shared_from_this(), cost);
    
}

void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        return;
    }

    // Add to the list of arrival flights
    arrivalFlights.push_back(flight);

    // Add to arrival graphs
    arrivalFlights_distance->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getDistance());
    arrivalFlights_cost->addEdge(flight->getDepartureAirport(), shared_from_this(), flight->getCost());

}

void Airport::addFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        return;
    }

    // Add the flight to both departure and arrival flights
    addDepartureFlight(flight);
    addArrivalFlight(flight);
}


void Airport::TakeOff(string flightNumber) {
    // Look for the flight in the list of departure flights by its flight number
    auto flight = std::find_if(departureFlights.begin(), departureFlights.end(),
                               [&flightNumber](const shared_ptr<Flight>& f) {
                                   return f->getFlightNumber() == flightNumber;
                               });

    // If the flight is found, change its status to "TAKEN_OFF"
    if (flight != departureFlights.end()) {
        // Update the status to TAKEN_OFF
        (*flight)->setStatus(FlightStatus::TAKEN_OFF);
        std::cout << "Flight " << flightNumber << " has taken off." << std::endl;
    } else {
        // If the flight is not found, show an error message
        std::cout << "Error: Flight " << flightNumber << " not found." << std::endl;
    }
}


void Airport::Land(string flightNumber) {
    // Find the flight in the list of arrival flights by its flight number
    auto flight = std::find_if(arrivalFlights.begin(), arrivalFlights.end(),
                               [&flightNumber](const shared_ptr<Flight>& f) {
                                   return f->getFlightNumber() == flightNumber;
                               });

    // Check if the flight exists and update its status
    if (flight != arrivalFlights.end()) {
        (*flight)->setStatus(FlightStatus::LANDED);
    } else {
        // std::cout << "Error: Flight " << flightNumber << " not found." << std::endl;
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
    : AirportNetwork_distance(true, true), AirportNetwork_cost(true, true) {
    // std::cout << "FlightNetwork instance created." << std::endl;
}
// Add Airport
void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    for (const auto& a : Airports) {
        std::cout << a->getName() << " ";
    }

    if (!hasAirport(airport->getName())) {
        Airports.push_back(airport);
        AirportNetwork_distance.addVertex(airport);
        AirportNetwork_cost.addVertex(airport);
    } else {
        // std::cout << "Airport " << airport->getName() << " already exists." << std::endl;
    }
}




// Add Flight
void FlightNetwork::addFlight(shared_ptr<Flight> flight) {
    auto departureAirport = flight->getDepartureAirport();
    auto destinationAirport = flight->getDestinationAirport();


    // Ensure departure and destination airports are in the network
    if (!hasAirport(departureAirport->getName())) {
        addAirport(departureAirport); 
    }
    if (!hasAirport(destinationAirport->getName())) {
        addAirport(destinationAirport); 
    }

    // Add flight to the airports and graphs
    departureAirport->addDepartureFlight(flight);
    destinationAirport->addArrivalFlight(flight);



    // Update graphs with new flight data
    AirportNetwork_distance.addEdge(departureAirport, destinationAirport, flight->getDistance());
    AirportNetwork_cost.addEdge(departureAirport, destinationAirport, flight->getCost());
}

// Check if Airport Exists
bool FlightNetwork::hasAirport(string name) {
    for (const auto& airport : Airports) {
        if (airport->getName() == name) {
            return true;
        }
    }
    return false;
}





// Check if Flight Exists
bool FlightNetwork::hasFlight(string flightNumber) {
    for (const auto& airport : Airports) {
        if (airport->hasFlight(flightNumber)) {
            return true;
        }
    }
    return false;
}



// Get Airport by Name
shared_ptr<Airport> FlightNetwork::getAirport(string name) {
    for (const auto& airport : Airports) {
        if (airport->getName() == name) {
            return airport;
        }
    }
    return nullptr;
}











// Get Flight by Flight Number
shared_ptr<Flight> FlightNetwork::getFlight(string flightNumber) {
    for (const auto& airport : Airports) {
        auto flight = airport->getFlight(flightNumber);
        if (flight) {
            return flight;
        }
    }
    return nullptr;
}

// Get Shortest Path
vector<shared_ptr<Airport>> FlightNetwork::getShortestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    auto path = AirportNetwork_distance.shortestPath(
        make_shared<Vertex<shared_ptr<Airport>>>(source),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)
    );

    vector<shared_ptr<Airport>> result;
    for (const auto& vertex : path) {
        result.push_back(vertex->getData());
    }
    return result;
}

// Get Cheapest Path
vector<shared_ptr<Airport>> FlightNetwork::getCheapestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    auto path = AirportNetwork_cost.shortestPath(
        make_shared<Vertex<shared_ptr<Airport>>>(source),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)
    );

    vector<shared_ptr<Airport>> result;
    for (const auto& vertex : path) {
        result.push_back(vertex->getData());
    }
    return result;
}

// Get Flight Plan
vector<shared_ptr<Airport>> FlightNetwork::getFlightPlan(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    auto shortest = getShortestPath(source, destination);
    auto cheapest = getCheapestPath(source, destination);

    if (shortest == cheapest) {
        return shortest;
    }

    // Use a dummy getEdgeWeight method as the original class lacks this feature
    auto sourceVertex = make_shared<Vertex<shared_ptr<Airport>>>(source);
    auto destinationVertex = make_shared<Vertex<shared_ptr<Airport>>>(destination);
    int shortestDistance = AirportNetwork_distance.getEdge(sourceVertex->getData(), destinationVertex->getData())->getWeight();
    int cheapestCost = AirportNetwork_cost.getEdge(sourceVertex->getData(), destinationVertex->getData())->getWeight();

    double percentageIncreaseShortest = (shortestDistance / (double)cheapestCost - 1) * 100;
    double percentageIncreaseCheapest = (cheapestCost / (double)shortestDistance - 1) * 100;

    return percentageIncreaseShortest < percentageIncreaseCheapest ? shortest : cheapest;
}

// Get All Flights
vector<shared_ptr<Flight>> FlightNetwork::getAllFlights() {
    unordered_set<shared_ptr<Flight>> uniqueFlights;
    for (const auto& airport : Airports) {
        for (const auto& flight : airport->getAllFlights()) {
            uniqueFlights.insert(flight);
        }
    }
    return vector<shared_ptr<Flight>>(uniqueFlights.begin(), uniqueFlights.end());
}

// Get All Airports
vector<shared_ptr<Airport>> FlightNetwork::getAllAirports() {
    return Airports;
}

// Get Busiest Airport
shared_ptr<Airport> FlightNetwork::getBusiestAirport() {
    shared_ptr<Airport> busiest = nullptr;
    int maxFlights = 0;

    for (const auto& airport : Airports) {
        int totalFlights = airport->getAllFlights().size();
        if (totalFlights > maxFlights) {
            maxFlights = totalFlights;
            busiest = airport;
        }
    }

    return busiest;
}

// Get Lamest Airport
shared_ptr<Airport> FlightNetwork::getLamestAirport() {
    shared_ptr<Airport> lamest = nullptr;
    int minFlights = INT_MAX;

    for (const auto& airport : Airports) {
        int totalFlights = airport->getAllFlights().size();
        if (totalFlights < minFlights) {
            minFlights = totalFlights;
            lamest = airport;
        }
    }

    return lamest;
}


// Optimize Graph
shared_ptr<Graph<shared_ptr<Airport>>> FlightNetwork::OptimizedGraph(bool distance) {
    return distance ? make_shared<Graph<shared_ptr<Airport>>>(AirportNetwork_distance)
                    : make_shared<Graph<shared_ptr<Airport>>>(AirportNetwork_cost);
}

// Alternate Route for a Flight
vector<shared_ptr<Airport>> FlightNetwork::alternateRouteForFlight(shared_ptr<Flight> flight) {
    auto source = flight->getDepartureAirport();
    auto destination = flight->getDestinationAirport();

    auto shortest = getShortestPath(source, destination);
    auto cheapest = getCheapestPath(source, destination);

    if (shortest == cheapest) {
        return shortest;
    }

    double originalDistance = flight->getDistance();
    double originalCost = flight->getCost();

    int newDistance = AirportNetwork_distance.getEdge(
        make_shared<Vertex<shared_ptr<Airport>>>(source)->getData(),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)->getData()
    )->getWeight();

    int newCost = AirportNetwork_cost.getEdge(
        make_shared<Vertex<shared_ptr<Airport>>>(source)->getData(),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)->getData()
    )->getWeight();

    double percentageIncrease = ((newDistance - originalDistance) / originalDistance * 100) +
                                ((newCost - originalCost) / originalCost * 100);

    return percentageIncrease < 0 ? shortest : cheapest;
}

// Airports Reachable
vector<shared_ptr<Airport>> FlightNetwork::AirportsReachable(shared_ptr<Airport> airport) {
    auto connectedComponents = AirportNetwork_distance.connectedComponents();

    vector<shared_ptr<Airport>> reachableAirports;
    for (const auto& component : connectedComponents) {
        for (const auto& vertex : component) {
            if (vertex->getData() == airport) {
                for (const auto& reachable : component) {
                    reachableAirports.push_back(reachable->getData());
                }
                return reachableAirports;
            }
        }
    }
    return reachableAirports;
}
