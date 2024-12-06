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
    // std::cout << "-Network----------------------------------------------------" << std::endl;

    // // Debugging: Print all flights for each airport
    // for (const auto& airport : Airports) {
    //     std::cout << "Airport: " << airport->getName() << std::endl;
    //     for (const auto& flight : airport->getAllFlights()) {
    //         std::cout << "  Flight: " << flight->getFlightNumber()
    //                   << " - Distance: " << flight->getDistance()
    //                   << " - Cost: " << flight->getCost() 
    //                   << " - Src: " << flight->getDepartureAirport()->getName() 
    //                   << " - Dst: " << flight->getDestinationAirport()->getName() << std::endl;
    //     }
    // }
    vector<double> distances(Airports.size(), std::numeric_limits<double>::infinity());
    vector<shared_ptr<Airport>> previous(Airports.size(), nullptr);
    vector<bool> visited(Airports.size(), false);
    
    queue<shared_ptr<Airport>> q;
    
    distances[0] = 0.0;  // Assuming source airport is at index 0 in Airports vector
    q.push(source);

    while (!q.empty()) {
        shared_ptr<Airport> currentAirport = q.front();
        q.pop();
        
        int currentIndex = -1;
        // Find the index of the current airport in Airports vector
        for (int i = 0; i < Airports.size(); ++i) {
            if (Airports[i] == currentAirport) {
                currentIndex = i;
                break;
            }
        }
        
        if (currentIndex == -1) continue;  // Skip if airport not found

        if (visited[currentIndex]) continue;
        visited[currentIndex] = true;

        if (currentAirport == destination) {
            vector<shared_ptr<Airport>> path;
            while (currentAirport != nullptr) {
                path.push_back(currentAirport);
                currentAirport = previous[currentIndex];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& flight : currentAirport->getAllFlights()) {
            shared_ptr<Airport> neighborAirport = flight->getDestinationAirport();
            int neighborIndex = -1;
            for (int i = 0; i < Airports.size(); ++i) {
                if (Airports[i] == neighborAirport) {
                    neighborIndex = i;
                    break;
                }
            }

            double newDistance = distances[currentIndex] + flight->getDistance();
            if (newDistance < distances[neighborIndex]) {
                distances[neighborIndex] = newDistance;
                previous[neighborIndex] = currentAirport;
                q.push(neighborAirport);
            }
        }
    }

    return {};  
}




vector<shared_ptr<Airport>> FlightNetwork::getCheapestPath(shared_ptr<Airport> source, shared_ptr<Airport> destination) {


    // Min-heap priority queue to store airports with their cumulative cost
    auto compare = [](const std::pair<shared_ptr<Airport>, double>& a, const std::pair<shared_ptr<Airport>, double>& b) {
        return a.second > b.second;  // We want the lowest cost to be processed first
    };

    std::priority_queue<std::pair<shared_ptr<Airport>, double>, std::vector<std::pair<shared_ptr<Airport>, double>>, decltype(compare)> pq(compare);
    
    // Store the cost to reach each airport (initialize to infinity)
    std::unordered_map<shared_ptr<Airport>, double> airportCosts;
    airportCosts[source] = 0.0;

    // Map to store the previous airport for path reconstruction
    std::unordered_map<shared_ptr<Airport>, shared_ptr<Airport>> previousAirport;

    pq.push({source, 0.0});  // Start with the source airport with a cost of 0

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        auto currentAirport = current.first;
        double currentCost = current.second;

        // If we reached the destination, reconstruct the path
        if (currentAirport == destination) {
            std::vector<shared_ptr<Airport>> result;
            while (currentAirport != nullptr) {
                result.push_back(currentAirport);
                currentAirport = previousAirport[currentAirport];
            }
            std::reverse(result.begin(), result.end());
            return result;
        }

        // Explore all neighboring airports (flights)
        for (const auto& flight : currentAirport->getAllFlights()) {
            auto neighborAirport = flight->getDestinationAirport();
            double newCost = currentCost + flight->getCost();

            // If we found a cheaper way to get to the neighbor, update the cost and push it to the queue
            if (airportCosts.find(neighborAirport) == airportCosts.end() || newCost < airportCosts[neighborAirport]) {
                airportCosts[neighborAirport] = newCost;
                pq.push({neighborAirport, newCost});
                previousAirport[neighborAirport] = currentAirport;
            }
        }
    }

    return {};  // Return an empty path if no path is found
}




// Get Flight Plan
vector<shared_ptr<Airport>> FlightNetwork::getFlightPlan(shared_ptr<Airport> source, shared_ptr<Airport> destination) {
    vector<vector<shared_ptr<Airport>>> allPaths;  // Will hold all the paths found
    vector<shared_ptr<Airport>> currentPath;       // Tracks the current path during exploration
    vector<shared_ptr<Airport>> reachableAirports = AirportsReachable(source);  // Get reachable airports from source

    // Traverse all reachable airports from the source
    for (const auto& airport : reachableAirports) {
        // If the airport is the destination, create a path
        if (airport == destination) {
            currentPath.push_back(source);
            currentPath.push_back(airport);
            allPaths.push_back(currentPath);
            currentPath.clear();
        } else {
            // If it's not the destination, continue exploring
            vector<shared_ptr<Airport>> furtherReachableAirports = AirportsReachable(airport);
            for (const auto& nextAirport : furtherReachableAirports) {
                if (nextAirport == destination) {
                    currentPath.push_back(source);
                    currentPath.push_back(airport);
                    currentPath.push_back(nextAirport);
                    allPaths.push_back(currentPath);
                    currentPath.clear();
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
    return allPaths.empty() ? vector<shared_ptr<Airport>>{} : allPaths[0];
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
    shared_ptr<Airport> departureAirport = flight->getDepartureAirport();
    shared_ptr<Airport> destinationAirport = flight->getDestinationAirport();

    // Check for a direct flight
    auto directRoute = getCheapestPath(departureAirport, destinationAirport);

    // If a direct route exists, find an alternate route with a stopover
    if (directRoute.size() == 2) {
        // For this example, we're considering LAX as a stopover airport
        // You can modify this to any other logic to find a stopover airport

        shared_ptr<Airport> stopoverAirport = nullptr;

        // Look for possible stopover airports (from the departure airport)
        for (const auto& flight : departureAirport->getAllFlights()) {
            if (flight->getDestinationAirport() != destinationAirport) {
                stopoverAirport = flight->getDestinationAirport();
                break; // Break after finding the first available stopover
            }
        }

        // If a stopover airport is found, we return the alternate route (e.g., JFK -> LAX -> ORD)
        if (stopoverAirport) {
            auto firstLeg = getCheapestPath(departureAirport, stopoverAirport);
            auto secondLeg = getCheapestPath(stopoverAirport, destinationAirport);

            // Combine the two legs into one path (without repeating the stopover airport)
            firstLeg.pop_back();  // Remove the stopover from the end of the first leg
            firstLeg.insert(firstLeg.end(), secondLeg.begin(), secondLeg.end()); // Merge paths

            return firstLeg;
        }
    }

    // If no alternate route is found, return the direct route (size 2)
    return directRoute;
}



// Airports Reachable
vector<shared_ptr<Airport>> FlightNetwork::AirportsReachable(shared_ptr<Airport> airport) {
    vector<shared_ptr<Airport>> reachableAirports;  // List to store all reachable airports

    // Get the shortest path and cheapest path from the given airport to all other airports
    for (const auto& potentialDestination : Airports) {
        if (airport != potentialDestination) {
            // Find the shortest path
            vector<shared_ptr<Airport>> shortestPath = getShortestPath(airport, potentialDestination);
            if (!shortestPath.empty()) {
                // If a path exists, it's reachable via shortest path
                reachableAirports.push_back(potentialDestination);
            } else {
                // Find the cheapest path
                vector<shared_ptr<Airport>> cheapestPath = getCheapestPath(airport, potentialDestination);
                if (!cheapestPath.empty()) {
                    // If a path exists, it's reachable via cheapest path
                    reachableAirports.push_back(potentialDestination);
                }
            }
        }
    }

    // Return the list of reachable airports
    return reachableAirports;
}
