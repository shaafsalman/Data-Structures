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

    //     std::cout << "Creating Flight: " << std::endl;
    // std::cout << "Flight Number: " << flightNumber << std::endl;
    // std::cout << "Departure Airport: " << (departureAirport ? departureAirport->getName() : "null") << std::endl;
    // std::cout << "Destination Airport: " << (destinationAirport ? destinationAirport->getName() : "null") << std::endl;
    // std::cout << "Distance: " << distance << std::endl;
    // std::cout << "Cost: " << cost << std::endl;
    // std::cout << "Status: " << status << std::endl;
    //  if (departureAirport) {
    //     departureAirport->addDepartureFlight(shared_ptr<Flight>(this));
    // }
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
    // Solution:
        return status;

}

void Flight::setStatus(FlightStatus newStatus) {
    this->status = newStatus;
}


/***************************************** AIRPORT CLASS *****************************************/
Airport::Airport()
    : name(""), city(""), country(""),
      arrivalFlights_distance(make_shared<Graph<shared_ptr<Airport>>>(true, true)),
      arrivalFlights_cost(make_shared<Graph<shared_ptr<Airport>>>(true, true)),
      departureFlights_distance(make_shared<Graph<shared_ptr<Airport>>>(true, true)),
      departureFlights_cost(make_shared<Graph<shared_ptr<Airport>>>(true, true)) {
}

Airport::Airport(string name, string city, string country)
    : name(std::move(name)), city(std::move(city)), country(std::move(country)),
      arrivalFlights_distance(make_shared<Graph<shared_ptr<Airport>>>(true, true)),
      arrivalFlights_cost(make_shared<Graph<shared_ptr<Airport>>>(true, true)),
      departureFlights_distance(make_shared<Graph<shared_ptr<Airport>>>(true, true)),
      departureFlights_cost(make_shared<Graph<shared_ptr<Airport>>>(true, true)) {
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
    if (!destination || flightNumber.empty()) {
        return;
    }

    auto flight = make_shared<Flight>(std::move(flightNumber), shared_from_this(), destination, distance, cost, status);
    departureFlights.push_back(flight);

    try {
        departureFlights_distance->addVertex(shared_from_this());
        departureFlights_distance->addVertex(destination);
        departureFlights_distance->addEdge(shared_from_this(), destination, distance);

        departureFlights_cost->addVertex(shared_from_this());
        departureFlights_cost->addVertex(destination);
        departureFlights_cost->addEdge(shared_from_this(), destination, cost);
    } catch (const exception& e) {
        cerr << "Error adding departure flight to graphs: " << e.what() << endl;
    }
}

void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    if (!source || flightNumber.empty()) {
        return;
    }

    auto flight = make_shared<Flight>(std::move(flightNumber), source, shared_from_this(), distance, cost, status);
    arrivalFlights.push_back(flight);

    try {
        arrivalFlights_distance->addVertex(source);
        arrivalFlights_distance->addVertex(shared_from_this());
        arrivalFlights_distance->addEdge(source, shared_from_this(), distance);

        arrivalFlights_cost->addVertex(source);
        arrivalFlights_cost->addVertex(shared_from_this());
        arrivalFlights_cost->addEdge(source, shared_from_this(), cost);
    } catch (const exception& e) {
        cerr << "Error adding arrival flight to graphs: " << e.what() << endl;
    }
}

// A unified method to add departure flights using a shared_ptr<Flight>


void Airport::addDepartureFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        
        return;
    }
    

    // Add to the list of departure flights
    departureFlights.push_back(flight);
    std::cout << "Added Departure Flight: " << flight->getFlightNumber() 
              << " from " << flight->getDepartureAirport()->getName()
              << " to " << flight->getDestinationAirport()->getName() << std::endl;

    // Add to departure graphs if not already present
    try {
        // Ensure the source and destination airports are added to the graph before adding the edge
        departureFlights_distance->addVertex(flight->getDepartureAirport());
        departureFlights_distance->addVertex(flight->getDestinationAirport());
        
        departureFlights_distance->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getDistance());
        departureFlights_cost->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getCost());
    } catch (const std::exception& e) {
        std::cerr << "Error adding edge to departure graph: " << e.what() << std::endl;
    }
}

void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        return;
    }

    // Add to the list of arrival flights
    arrivalFlights.push_back(flight);
    std::cout << "Added Arrival Flight: " << flight->getFlightNumber() 
              << " from " << flight->getDepartureAirport()->getName()
              << " to " << flight->getDestinationAirport()->getName() << std::endl;

    // Add to arrival graphs if not already present
    try {
        // Ensure the source and destination airports are added to the graph before adding the edge
        arrivalFlights_distance->addVertex(flight->getDepartureAirport());
        arrivalFlights_distance->addVertex(flight->getDestinationAirport());

        arrivalFlights_distance->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getDistance());
        arrivalFlights_cost->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getCost());
    } catch (const std::exception& e) {
        std::cerr << "Error adding edge to arrival graph: " << e.what() << std::endl;
    }
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
    // Find the flight in the departure list
    auto it = std::find_if(departureFlights.begin(), departureFlights.end(),
                           [&flightNumber](const shared_ptr<Flight>& flight) {
                               return flight->getFlightNumber() == flightNumber;
                           });

    // Debugging output
    if (it != departureFlights.end()) {
        auto flight = *it;
        
        // Check if flight status changes
        flight->setStatus(FlightStatus::TAKEN_OFF);
        

    } else {
    }
}



void Airport::Land(string flightNumber) {
    // Find the flight in the list of arrival flights by its flight number
    auto it = std::find_if(arrivalFlights.begin(), arrivalFlights.end(),
                           [&flightNumber](const shared_ptr<Flight>& flight) {
                               return flight->getFlightNumber() == flightNumber;
                           });

    if (it != arrivalFlights.end()) {
        auto flight = *it;

        // Check the current status before updating
        if (flight->getStatus() != FlightStatus::SCHEDULED && flight->getStatus() != FlightStatus::TAKEN_OFF) {
            return;
        }

        // Update the status to LANDED
        flight->setStatus(FlightStatus::LANDED);
    } else {
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
}
// Add Airport

void FlightNetwork::addAirport(shared_ptr<Airport> airport) {
    if (!hasAirport(airport->getName())) {
        Airports.push_back(airport);
        AirportNetwork_distance.addVertex(airport);
        AirportNetwork_cost.addVertex(airport);
    }
}



// Add Flight
void FlightNetwork::addFlight(shared_ptr<Flight> flight) {
    auto departureAirport = flight->getDepartureAirport();
    auto destinationAirport = flight->getDestinationAirport();

    // Ensure airports are added to the network if not already added
    addAirport(departureAirport);
    addAirport(destinationAirport);

    // Add the flight to the airports' departure and arrival lists
    departureAirport->addDepartureFlight(flight);
    destinationAirport->addArrivalFlight(flight);

    // Add both directions in distance and cost networks
    AirportNetwork_distance.addEdge(departureAirport, destinationAirport, flight->getDistance());
    AirportNetwork_cost.addEdge(departureAirport, destinationAirport, flight->getCost());
    
    // Add reverse direction as well for bidirectional graph
    AirportNetwork_distance.addEdge(destinationAirport, departureAirport, flight->getDistance());
    AirportNetwork_cost.addEdge(destinationAirport, departureAirport, flight->getCost());
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


vector<shared_ptr<Airport>> FlightNetwork::getShortestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    std::cout << "Finding shortest path from " << source->getName() 
              << " to " << destination->getName() << std::endl;

    // Debugging: Print out all airport connections (edges)
    std::cout << "Graph connections: " << std::endl;
    for (const auto& airport : Airports) {
        std::cout << "Airport: " << airport->getName() << " has connections to: ";
        for (const auto& flight : airport->getAllFlights()) {
            std::cout << flight->getDestinationAirport()->getName() << " ";
        }
        std::cout << std::endl;
    }

    // Get the shortest path from the AirportNetwork_distance graph
    auto path = AirportNetwork_distance.shortestPath(
        make_shared<Vertex<shared_ptr<Airport>>>(source),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)
    );

    // Debugging: Check if the path is found
    if (path.empty()) {
        std::cerr << "No path found between " << source->getName() 
                  << " and " << destination->getName() << std::endl;
        return {};  // Return an empty path if no path is found
    }

    // Debugging: Print the raw path as returned from shortestPath (in terms of vertices)
    std::cout << "Raw shortest path (in terms of vertices): ";
    for (const auto& vertex : path) {
        std::cout << vertex->getData()->getName() << " ";
    }
    std::cout << std::endl;

    // Convert the path from vertices to airports
    vector<shared_ptr<Airport>> result;
    for (const auto& vertex : path) {
        result.push_back(vertex->getData());
    }

    // Debugging: Print the airports in the final shortest path
    std::cout << "Shortest path airports: ";
    for (const auto& airport : result) {
        std::cout << airport->getName() << " ";
    }
    std::cout << std::endl;

    return result;
}



vector<shared_ptr<Airport>> FlightNetwork::getCheapestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    std::cout << "Finding cheapest path from " << source->getName() 
              << " to " << destination->getName() << std::endl;

    // Get the cheapest path from the AirportNetwork_cost
    auto path = AirportNetwork_cost.shortestPath(
        make_shared<Vertex<shared_ptr<Airport>>>(source),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)
    );

    // Debugging: Print the raw path as returned from shortestPath
    std::cout << "Raw cheapest path (in terms of vertices): ";
    for (const auto& vertex : path) {
        std::cout << vertex->getData()->getName() << " ";
    }
    std::cout << std::endl;

    // Convert the path from vertices to airports
    vector<shared_ptr<Airport>> result;
    for (const auto& vertex : path) {
        result.push_back(vertex->getData());
    }

    // Debugging: Print the airports in the final cheapest path
    std::cout << "Cheapest path airports: ";
    for (const auto& airport : result) {
        std::cout << airport->getName() << " ";
    }
    std::cout << std::endl;

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

    // Iterate over all airports and find the one with the most flights
    for (const auto& airport : Airports) {
        int totalFlights = airport->getAllFlights().size();
        if (totalFlights > maxFlights) {
            maxFlights = totalFlights;
            busiest = airport;
        }
    }

    // Check if no busiest airport is found (in case no flights exist)
    if (busiest == nullptr) {
    }

    return busiest;
}

shared_ptr<Airport> FlightNetwork::getLamestAirport() {
    shared_ptr<Airport> lamest = nullptr;
    int minFlights = INT_MAX;

    // Iterate over all airports and find the one with the least flights
    for (const auto& airport : Airports) {
        int totalFlights = airport->getAllFlights().size();
        if (totalFlights < minFlights) {
            minFlights = totalFlights;
            lamest = airport;
        }
    }

    // Check if no lamest airport is found (in case no flights exist)
    if (lamest == nullptr) {
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
    std::cout << "-----------------------------------------------------" << std::endl;

    // Debugging: Print flights with weights and costs for each airport
    for (const auto& airport : Airports) {
        std::cout << "Airport: " << airport->getName() << std::endl;
        for (const auto& flight : airport->getAllFlights()) {
            std::cout << "  Flight: " << flight->getFlightNumber()
                      << " - Distance: " << flight->getDistance()
                      << " - Cost: " << flight->getCost() 
                      << " - Src: " << flight->getDepartureAirport()->getName() 
                      << " - Dst: " << flight->getDestinationAirport()->getName() << std::endl;
        }
    }
    std::cout << "-----------------------------------------------------" << std::endl;

    auto source = flight->getDepartureAirport();
    auto destination = flight->getDestinationAirport();

    std::cout << "Source Airport: " << source->getName() << std::endl;
    std::cout << "Destination Airport: " << destination->getName() << std::endl;

    // Get the shortest and cheapest paths
    auto shortest = getShortestPath(source, destination);
    auto cheapest = getCheapestPath(source, destination);

    // Debugging: Print the size of the paths
    std::cout << "Shortest route size: " << shortest.size() << std::endl;
    std::cout << "Cheapest route size: " << cheapest.size() << std::endl;

    // Print the airports in both routes for further clarity
    std::cout << "Shortest route airports: ";
    for (const auto& airport : shortest) {
        std::cout << airport->getName() << " ";
    }
    std::cout << std::endl;

    std::cout << "Cheapest route airports: ";
    for (const auto& airport : cheapest) {
        std::cout << airport->getName() << " ";
    }
    std::cout << std::endl;

    // Ensure both paths are valid and non-empty
    if (shortest.empty() || cheapest.empty()) {
        std::cerr << "One of the paths is empty, returning empty route" << std::endl;
        return {};  // Or handle appropriately
    }

    // If both paths are identical, return the shortest one
    if (shortest == cheapest) {
        std::cout << "Both paths are identical, returning the shortest one." << std::endl;
        return shortest;
    }

    double originalDistance = flight->getDistance();
    double originalCost = flight->getCost();

    std::cout << "Original Flight Distance: " << originalDistance << " km" << std::endl;
    std::cout << "Original Flight Cost: " << originalCost << " USD" << std::endl;

    // Get new distance and cost for alternate routes
    int newDistance = 0;
    int newCost = 0;

    // Ensure we get the correct edge for distance and cost
    auto edgeDistance = AirportNetwork_distance.getEdge(
        make_shared<Vertex<shared_ptr<Airport>>>(source)->getData(),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)->getData()
    );
    
    auto edgeCost = AirportNetwork_cost.getEdge(
        make_shared<Vertex<shared_ptr<Airport>>>(source)->getData(),
        make_shared<Vertex<shared_ptr<Airport>>>(destination)->getData()
    );

    if (edgeDistance && edgeCost) {
        newDistance = edgeDistance->getWeight();
        newCost = edgeCost->getWeight();
    } else {
        std::cerr << "Error: Unable to retrieve edge information for the given airports." << std::endl;
        return {};  // Or handle appropriately if edges are missing
    }

    std::cout << "New Distance: " << newDistance << " km" << std::endl;
    std::cout << "New Cost: " << newCost << " USD" << std::endl;

    // Calculate the percentage increase in distance and cost
    double percentageIncrease = ((newDistance - originalDistance) / originalDistance * 100) +
                                ((newCost - originalCost) / originalCost * 100);

    std::cout << "Percentage increase: " << percentageIncrease << "%" << std::endl;

    // Return the path with the least increase in distance and cost
    if (percentageIncrease < 0) {
        std::cout << "Choosing shortest path as it is cheaper." << std::endl;
        return shortest;  // Choose shortest path if cheaper
    } else {
        std::cout << "Choosing cheapest path as it is better value." << std::endl;
        return cheapest;  // Otherwise, choose cheapest path
    }
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
