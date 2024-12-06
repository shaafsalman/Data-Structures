#include "FlightNetwork.h"

/***************************************** FLIGHT CLASS *****************************************/
Flight::Flight() {
    flightNumber = "";
    distance = 0;
    cost = 0;
    status = FlightStatus::SCHEDULED;
}

Flight::Flight(string flightNumber, shared_ptr<Airport> departureAirport, shared_ptr<Airport> destinationAirport, int distance, int cost, FlightStatus status) {
    this->flightNumber = flightNumber;
    this->departureAirport = departureAirport;
    this->destinationAirport = destinationAirport;
    this->distance = distance;
    this->cost = cost;
    this->status = status;
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
Airport::Airport() {
    name = "";
    city = "";
    country = "";
    arrivalFlights_distance = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    arrivalFlights_cost = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    departureFlights_distance = make_shared<Graph<shared_ptr<Airport>>>(true, true);
    departureFlights_cost = make_shared<Graph<shared_ptr<Airport>>>(true, true);
}

Airport::Airport(string name, string city, string country) {
    this->name = name;
    this->city = city;
    this->country = country;
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
    if (flightNumber.empty() || !destination) {
        return;
    }

    auto flight = make_shared<Flight>(flightNumber, shared_from_this(), destination, distance, cost, status);
    departureFlights.push_back(flight);

    departureFlights_distance->addVertex(shared_from_this());
    departureFlights_distance->addVertex(destination);
    departureFlights_distance->addEdge(shared_from_this(), destination, distance);

    departureFlights_cost->addVertex(shared_from_this());
    departureFlights_cost->addVertex(destination);
    departureFlights_cost->addEdge(shared_from_this(), destination, cost);
}
void Airport::addArrivalFlight(string flightNumber, shared_ptr<Airport> source, int cost, int distance, FlightStatus status) {
    if (flightNumber.empty() || !source) {
        return;
    }

    auto flight = make_shared<Flight>(flightNumber, source, shared_from_this(), distance, cost, status);
    arrivalFlights.push_back(flight);

    arrivalFlights_distance->addVertex(source);
    arrivalFlights_distance->addVertex(shared_from_this());
    arrivalFlights_distance->addEdge(source, shared_from_this(), distance);

    arrivalFlights_cost->addVertex(source);
    arrivalFlights_cost->addVertex(shared_from_this());
    arrivalFlights_cost->addEdge(source, shared_from_this(), cost);
}
void Airport::addDepartureFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        return;
    }

    departureFlights.push_back(flight);

    departureFlights_distance->addVertex(flight->getDepartureAirport());
    departureFlights_distance->addVertex(flight->getDestinationAirport());
    departureFlights_distance->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getDistance());

    departureFlights_cost->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getCost());
}
void Airport::addArrivalFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        return;
    }

    arrivalFlights.push_back(flight);

    arrivalFlights_distance->addVertex(flight->getDepartureAirport());
    arrivalFlights_distance->addVertex(flight->getDestinationAirport());
    arrivalFlights_distance->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getDistance());

    arrivalFlights_cost->addEdge(flight->getDepartureAirport(), flight->getDestinationAirport(), flight->getCost());
}
void Airport::addFlight(shared_ptr<Flight> flight) {
    if (!flight) {
        return;
    }

    addDepartureFlight(flight);
    addArrivalFlight(flight);
}
void Airport::TakeOff(string flightNumber) {
    for (auto& flight : departureFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            flight->setStatus(FlightStatus::TAKEN_OFF);
            return;
        }
    }
}
void Airport::Land(string flightNumber) {
    for (auto& flight : arrivalFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            if (flight->getStatus() == FlightStatus::SCHEDULED || flight->getStatus() == FlightStatus::TAKEN_OFF) {
                flight->setStatus(FlightStatus::LANDED);
            }
            return;
        }
    }
}
bool Airport::hasFlight(string flightNumber) {
    for (auto& flight : departureFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return true;
        }
    }
    for (auto& flight : arrivalFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return true;
        }
    }
    return false;
}
shared_ptr<Flight> Airport::getFlight(string flightNumber) {
    for (auto& flight : departureFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    for (auto& flight : arrivalFlights) {
        if (flight->getFlightNumber() == flightNumber) {
            return flight;
        }
    }
    return nullptr;
}
vector<shared_ptr<Flight>> Airport::getAllFlights() {
    vector<shared_ptr<Flight>> allFlights;
    
    for (auto& flight : departureFlights) {
        allFlights.push_back(flight);
    }
    
    for (auto& flight : arrivalFlights) {
        allFlights.push_back(flight);
    }
    
    return allFlights;
}


/************************************** FLIGHTNETWORK CLASS *****************************************/

FlightNetwork::FlightNetwork() {
    AirportNetwork_distance = Graph<shared_ptr<Airport>>(true, true);
    AirportNetwork_cost = Graph<shared_ptr<Airport>>(true, true);
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

    // Ensure airports are added to the network
    addAirport(departureAirport);
    addAirport(destinationAirport);

    // Add flight to airports
    departureAirport->addDepartureFlight(flight);
    destinationAirport->addArrivalFlight(flight);

    //edges in both directions
    AirportNetwork_distance.addEdge(departureAirport, destinationAirport, flight->getDistance());
    AirportNetwork_cost.addEdge(departureAirport, destinationAirport, flight->getCost());

    AirportNetwork_distance.addEdge(destinationAirport, departureAirport, flight->getDistance());
    AirportNetwork_cost.addEdge(destinationAirport, departureAirport, flight->getCost());
}





// Check if Airport Exists
bool FlightNetwork::hasAirport(string name) {
    for (auto& airport : Airports) {
        if (airport->getName() == name) {
            return true;
        }
    }
    return false;
}

// Check if Flight Exists
bool FlightNetwork::hasFlight(string flightNumber) {
    for (auto& airport : Airports) {
        if (airport->hasFlight(flightNumber)) {
            return true;
        }
    }
    return false;
}

// Get Airport by Name
shared_ptr<Airport> FlightNetwork::getAirport(string name) {
    for (auto& airport : Airports) {
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
    vector<shared_ptr<Airport>> airports = Airports;
    vector<int> distances(airports.size(), INT_MAX);
    vector<int> previous(airports.size(), -1);
    vector<bool> visited(airports.size(), false);

    auto getIndex = [&](shared_ptr<Airport> airport) {
        for (int i = 0; i < airports.size(); ++i)
         {
            if (airports[i] == airport) {return i;}
         }
         return -1;
    };

    int sourceIndex = getIndex(source);
    distances[sourceIndex] = 0.0;

    for (size_t i = 0; i < airports.size(); ++i) {
        // Find the unvisited airport with the smallest distance
        int currentIndex = -1;
        for (size_t j = 0; j < airports.size(); ++j) {
            if (!visited[j] && (currentIndex == -1 || distances[j] < distances[currentIndex])) {
                currentIndex = j;
            }
        }

        if (currentIndex == -1 || distances[currentIndex] == INT_MAX) {
            break;
        }

        visited[currentIndex] = true;
        auto currentAirport = airports[currentIndex];

        for (const auto& flight : currentAirport->getAllFlights()) {
            auto neighbor = flight->getDestinationAirport();
            int neighborIndex = getIndex(neighbor);
            double newDistance = distances[currentIndex] + flight->getDistance();

            if (newDistance < distances[neighborIndex]) {
                distances[neighborIndex] = newDistance;
                previous[neighborIndex] = currentIndex;
            }
        }
    }

    // Reconstruct the path
    vector<shared_ptr<Airport>> path;
    for (int at = getIndex(destination); at != -1; at = previous[at]) {
        path.push_back(airports[at]);
    }
    std::reverse(path.begin(), path.end());

    if (!path.empty() && path.front() == source) {
        return path;
    }
    return {};
}

vector<shared_ptr<Airport>> FlightNetwork::getCheapestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    vector<shared_ptr<Airport>> airports = Airports;
    vector<double> costs(airports.size(), std::numeric_limits<double>::infinity());
    vector<int> previous(airports.size(), -1);
    vector<bool> visited(airports.size(), false);

     auto getIndex = [&](shared_ptr<Airport> airport) {
        for (int i = 0; i < airports.size(); ++i)
         {
            if (airports[i] == airport) {return i;}
         }
         return -1;
    };

    int sourceIndex = getIndex(source);
    costs[sourceIndex] = 0.0;

    for (size_t i = 0; i < airports.size(); ++i) {
        int currentIndex = -1;
        for (size_t j = 0; j < airports.size(); ++j) {
            if (!visited[j] && (currentIndex == -1 || costs[j] < costs[currentIndex])) {
                currentIndex = j;
            }
        }

        if (currentIndex == -1 || distances[currentIndex] == INT_MAX) {
            break;
        }

        visited[currentIndex] = true;
        auto currentAirport = airports[currentIndex];

        // Process all flights from the current airport
        for (const auto& flight : currentAirport->getAllFlights()) {
            auto neighbor = flight->getDestinationAirport();
            int neighborIndex = getIndex(neighbor);
            double newCost = costs[currentIndex] + flight->getCost();

            if (newCost < costs[neighborIndex]) {
                costs[neighborIndex] = newCost;
                previous[neighborIndex] = currentIndex;
            }
        }
    }

    // Reconstruct the path
    vector<shared_ptr<Airport>> path;
    for (int at = getIndex(destination); at != -1; at = previous[at]) {
        path.push_back(airports[at]);
    }
    std::reverse(path.begin(), path.end());

    if (!path.empty() && path.front() == source) {
        return path;
    }
    return {};
}

// Get Flight Plan
vector<shared_ptr<Airport>> FlightNetwork::getFlightPlan(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    vector<vector<shared_ptr<Airport>>> allPaths;
    vector<shared_ptr<Airport>> currentPath;
    vector<shared_ptr<Airport>> reachableAirports = AirportsReachable(source);

    for (const auto& airport : reachableAirports) {
        if (airport == destination) {
            allPaths.push_back({source, airport});
        } else {
            for (const auto& nextAirport : AirportsReachable(airport)) {
                if (nextAirport == destination) {
                    allPaths.push_back({source, airport, nextAirport});
                }
            }
        }
    }
    
    // // For simplicity, the flight plan is just the first path found
    // // Print all paths
    // std::cout << "TOTAL PATHS: { ";
    // for (size_t i = 0; i < allPaths.size(); ++i) {
    //     std::cout << "P" << i + 1 << " : { ";
    //     for (size_t j = 0; j < allPaths[i].size(); ++j) {
    //         std::cout << allPaths[i][j]->getName();
    //         if (j < allPaths[i].size() - 1) {
    //             std::cout << " -> ";
    //         }
    //     }
    //     std::cout << " }";
    //     if (i < allPaths.size() - 1) {
    //         std::cout << ", ";
    //     }
    // }
    // std::cout << " }\n";

    // // For this example, return the first path as the flight plan
    // std::cout << "FLIGHT PLAN = P1\n";

    return allPaths.empty() ? vector<shared_ptr<Airport>>() : allPaths[0];
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
    auto departureAirport = flight->getDepartureAirport();
    auto destinationAirport = flight->getDestinationAirport();
    auto directRoute = getCheapestPath(departureAirport, destinationAirport);

    if (directRoute.size() == 2) {
        shared_ptr<Airport> stopoverAirport = nullptr;

        for (const auto& flight : departureAirport->getAllFlights()) {
            if (flight->getDestinationAirport() != destinationAirport) {
                stopoverAirport = flight->getDestinationAirport();
                break;
            }
        }

        if (stopoverAirport) {
            auto firstLeg = getCheapestPath(departureAirport, stopoverAirport);
            auto secondLeg = getCheapestPath(stopoverAirport, destinationAirport);
            firstLeg.pop_back();
            firstLeg.insert(firstLeg.end(), secondLeg.begin(), secondLeg.end());
            return firstLeg;
        }
    }

    return directRoute;
}

// Airports Reachable
vector<shared_ptr<Airport>> FlightNetwork::AirportsReachable(shared_ptr<Airport> airport) {
    vector<shared_ptr<Airport>> reachableAirports;

    for (const auto& potentialDestination : Airports) {
        if (airport != potentialDestination) {
            if (!getShortestPath(airport, potentialDestination).empty()) {
                reachableAirports.push_back(potentialDestination);
            } else if (!getCheapestPath(airport, potentialDestination).empty()) {
                reachableAirports.push_back(potentialDestination);
            }
        }
    }

    return reachableAirports;
}