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
    std::cout << "Get Status of flight " << flightNumber << " " << this->status << std::endl;

    // Solution:
        return status;

}

void Flight::setStatus(FlightStatus newStatus) {
    status = newStatus;
    std::cout << "Set status of flight " << flightNumber << " to " << status << std::endl;
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
    if (!destination) {
        std::cout << "Error: Destination airport is null." << std::endl;
        return;
    }

    if (flightNumber.empty()) {
        std::cout << "Error: Flight number is empty." << std::endl;
        return;
    }

    // Create a new flight
    auto flight = make_shared<Flight>(flightNumber, shared_from_this(), destination, distance, cost, status);
    if (!flight) {
        std::cout << "Error: Failed to create flight." << std::endl;
        return;
    }

    // Add to the list of departure flights
    departureFlights.push_back(flight);
    std::cout << "Added departure flight: " << flightNumber << " from " << getName() << " to " << destination->getName() << std::endl;

    // Add to departure graphs
    try {
        departureFlights_distance->addEdge(shared_from_this(), destination, distance);
        departureFlights_cost->addEdge(shared_from_this(), destination, cost);
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to add edges to the departure graphs: " << e.what() << std::endl;
    }
}



void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    // Check for invalid parameters
    if (!source) {
        std::cout << "Error: Source airport is null." << std::endl;
        return;
    }

    if (flightNumber.empty()) {
        std::cout << "Error: Flight number is empty." << std::endl;
        return;
    }

    // Create a new flight
    auto flight = make_shared<Flight>(flightNumber, source, shared_from_this(), distance, cost, status);
    if (!flight) {
        std::cout << "Error: Failed to create flight." << std::endl;
        return;
    }

    // Add to the list of arrival flights
    arrivalFlights.push_back(flight);
    std::cout << "Added arrival flight: " << flightNumber << " from " << source->getName() << " to " << getName() << std::endl;

    // Add to arrival graphs
    try {
        arrivalFlights_distance->addEdge(source, shared_from_this(), distance);
        arrivalFlights_cost->addEdge(source, shared_from_this(), cost);
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to add edges to the arrival graphs: " << e.what() << std::endl;
    }
}







void Airport::addDepartureFlight(std::shared_ptr<Flight> flight) {
    if (!flight) {
        std::cout << "Error: Flight is null." << std::endl;
        return;
    }

    departureFlights.push_back(flight);
    std::cout << "Added departure flight: " << flight->getFlightNumber() 
              << " from " << getName() << " to " << flight->getDestinationAirport()->getName() << std::endl;

    try {
        auto thisAirport = shared_from_this(); // Ensure this is properly shared
        departureFlights_distance->addEdge(thisAirport, flight->getDestinationAirport(), flight->getDistance());
        departureFlights_cost->addEdge(thisAirport, flight->getDestinationAirport(), flight->getCost());
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to add edges to the departure graphs: " << e.what() << std::endl;
    }
}

void Airport::addArrivalFlight(std::shared_ptr<Flight> flight) {
    if (!flight) {
        std::cout << "Error: Flight is null." << std::endl;
        return;
    }

    arrivalFlights.push_back(flight);
    std::cout << "Added arrival flight: " << flight->getFlightNumber() 
              << " from " << flight->getDepartureAirport()->getName() << " to " << getName() << std::endl;

    try {
        auto thisAirport = shared_from_this(); // Ensure this is properly shared
        arrivalFlights_distance->addEdge(flight->getDepartureAirport(), thisAirport, flight->getDistance());
        arrivalFlights_cost->addEdge(flight->getDepartureAirport(), thisAirport, flight->getCost());
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to add edges to the arrival graphs: " << e.what() << std::endl;
    }
}


void Airport::addFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        std::cout << "Error: Flight is null." << std::endl;
        return;
    }

    // Add the flight to both departure and arrival flights
    addDepartureFlight(flight);
    addArrivalFlight(flight);
}


void Airport::TakeOff(string flightNumber) {
    shared_ptr<Flight> flight = getFlight(flightNumber);  // Make sure this retrieves the correct flight
    if (flight) {
        std::cout << "Setting status of flight " << flightNumber << " to TAKEN_OFF." << std::endl;
        flight->setStatus(FlightStatus::TAKEN_OFF);  // This should modify the flight's status
        std::cout << "Flight " << flightNumber << " has taken off." << std::endl;
    } else {
        std::cout << "Flight not found: " << flightNumber << std::endl;
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
        std::cout << "Flight " << flightNumber << " has landed." << std::endl;
    } else {
        std::cout << "Error: Flight " << flightNumber << " not found." << std::endl;
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
    std::cout << "FlightNetwork instance created." << std::endl;
}
// Add Airport
void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    std::cout << "Current airports in network: ";
    for (const auto& a : Airports) {
        std::cout << a->getName() << " ";
    }
    std::cout << std::endl;

    if (!hasAirport(airport->getName())) {
        std::cout << "Adding airport: " << airport->getName() << std::endl;
        Airports.push_back(airport);
        AirportNetwork_distance.addVertex(airport);
        AirportNetwork_cost.addVertex(airport);
    } else {
        std::cout << "Airport " << airport->getName() << " already exists." << std::endl;
    }

    std::cout << "Airports after addition: ";
    for (const auto& a : Airports) {
        std::cout << a->getName() << " ";
    }
    std::cout << std::endl;
}




// Add Flight
void FlightNetwork::addFlight(shared_ptr<Flight> flight) {
    auto departureAirport = flight->getDepartureAirport();
    auto destinationAirport = flight->getDestinationAirport();

    cout << "Attempting to add flight " << flight->getFlightNumber() 
         << " from " << departureAirport->getName() 
         << " to " << destinationAirport->getName() << endl;

    // Ensure departure and destination airports are in the network
    if (!hasAirport(departureAirport->getName())) {
        cout << "Adding missing departure airport: " << departureAirport->getName() << endl;
        addAirport(departureAirport);  // Adds only if not present
    }
    if (!hasAirport(destinationAirport->getName())) {
        cout << "Adding missing destination airport: " << destinationAirport->getName() << endl;
        addAirport(destinationAirport);  // Adds only if not present
    }

    // Add flight to the airports and graphs
    departureAirport->addDepartureFlight(flight);
    destinationAirport->addArrivalFlight(flight);

    cout << "Adding flight " << flight->getFlightNumber() 
         << " to graph: from " << departureAirport->getName() 
         << " to " << destinationAirport->getName() << endl;

    // Update graphs with new flight data
    AirportNetwork_distance.addEdge(departureAirport, destinationAirport, flight->getDistance());
    AirportNetwork_cost.addEdge(departureAirport, destinationAirport, flight->getCost());
}

// Check if Airport Exists
bool FlightNetwork::hasAirport(string name) {
    std::cout << "Checking for airport: " << name << std::endl;
    for (const auto& airport : Airports) {
        std::cout << "Comparing with: " << airport->getName() << std::endl;
        if (airport->getName() == name) {
            std::cout << "Airport found: " << name << std::endl;
            return true;
        }
    }
    std::cout << "Airport not found: " << name << std::endl;
    return false;
}





// Check if Flight Exists
bool FlightNetwork::hasFlight(string flightNumber) {
    std::cout << "Checking for flight: " << flightNumber << std::endl;
    for (const auto& airport : Airports) {
        std::cout << "Checking flights at airport: " << airport->getName() << std::endl;
        if (airport->hasFlight(flightNumber)) {
            std::cout << "Flight found at airport: " << airport->getName() << std::endl;
            return true;
        }
    }
    std::cout << "Flight not found: " << flightNumber << std::endl;
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
