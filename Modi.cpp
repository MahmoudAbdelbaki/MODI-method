#include <bits/stdc++.h>

// Function to print a matrix
void printMatrix(const std::vector<std::vector<long long>>& matrix, const std::string& name) {
    std::cout << name << " matrix:\n";
    for (const auto& row : matrix) {
        for (long long val : row) {
            std::cout << val << "\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

// Function to print a vector
void printVector(const std::vector<long long>& vec, const std::string& name) {
    std::cout << name << " vector: ";
    for (long long val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

class TransportationProblem {
public:
    // Function to calculate potentials (u and v values) based on the current allocations
    static void calculateUV(const std::vector<std::vector<long long>>& cost,
                             const std::vector<std::vector<long long>>& alloc,
                             std::vector<long long>& u,
                             std::vector<long long>& v) {
        long long n = cost.size(); // Number of rows (sources)
        long long m = cost[0].size(); // Number of columns (destinations)

        // Fill u and v with minimum possible values
        u.assign(n, LLONG_MIN);
        v.assign(m, LLONG_MIN);

        u[0] = 0; // Arbitrary choice to fix one potential

        bool updated;
        do {
            updated = false;

            // Iterate over all allocated cells to calculate u and v
            for (long long i = 0; i < n; i++) {
                for (long long j = 0; j < m; j++) {
                    if (alloc[i][j] > 0) { // If there is an allocation in cell (i, j)
                        if (u[i] != LLONG_MIN && v[j] == LLONG_MIN) {
                            v[j] = cost[i][j] - u[i];
                            updated = true;
                        } else if (v[j] != LLONG_MIN && u[i] == LLONG_MIN) {
                            u[i] = cost[i][j] - v[j];
                            updated = true;
                        }
                    }
                }
            }
        } while (updated);

        // Print u and v vectors for debugging
        printVector(u, "U potentials");
        printVector(v, "V potentials");
    }

    // Function to calculate delta (opportunity costs) for all unallocated cells
    static void findDelta(const std::vector<std::vector<long long>>& cost,
                           const std::vector<std::vector<long long>>& alloc,
                           const std::vector<long long>& u,
                           const std::vector<long long>& v,
                           std::vector<std::vector<long long>>& delta) {
        long long n = cost.size();
        long long m = cost[0].size();

        for (long long i = 0; i < n; i++) {
            for (long long j = 0; j < m; j++) {
                if (alloc[i][j] == 0) { // Only calculate delta for unallocated cells
                    delta[i][j] = cost[i][j] - (u[i] + v[j]);
                } else {
                    delta[i][j] = 0;
                }
            }
        }

        // Print delta matrix for debugging
        printMatrix(delta, "Delta");
    }

    // Function to find the most negative delta
    static std::vector<long long> findMostNegativeDelta(const std::vector<std::vector<long long>>& delta) {
        long long minDelta = 0; // Minimum delta starts at 0
        std::vector<long long> position = {-1, -1}; // Position of the most negative delta

        for (long long i = 0; i < delta.size(); i++) {
            for (long long j = 0; j < delta[0].size(); j++) {
                if (delta[i][j] < minDelta) {
                    minDelta = delta[i][j];
                    position = {i, j};
                }
            }
        }
        return position; // Return position of the most negative delta
    }

    // Function to adjust allocations along a loop
    static void adjustAllocations(std::vector<std::vector<long long>>& alloc,
                                  const std::vector<std::vector<long long>>& loop) {
        // Find the smallest allocation in the loop on negative positions
        long long minAlloc = LLONG_MAX;
        for (long long k = 1; k < loop.size(); k += 2) {
            const auto& cell = loop[k];
            minAlloc = std::min(minAlloc, alloc[cell[0]][cell[1]]);
        }

        // Adjust allocations along the loop
        for (long long k = 0; k < loop.size(); k++) {
            const auto& cell = loop[k];
            if (k % 2 == 0) { // Add allocation on even positions
                alloc[cell[0]][cell[1]] += minAlloc;
            } else { // Subtract allocation on odd positions
                alloc[cell[0]][cell[1]] -= minAlloc;
            }
        }

        // Print updated allocation matrix
        printMatrix(alloc, "Updated Allocation");
    }

    // Function to find a loop starting from a given cell (simplified implementation)
    static std::vector<std::vector<long long>> findLoop(const std::vector<std::vector<long long>>& alloc,
                                                  long long startRow, long long startCol) {
        std::vector<std::vector<long long>> loop = {{startRow, startCol}};

        // Note: This is a simplified loop finding mechanism and may not work for all cases
        // A more robust implementation would require more complex logic

        return loop;
    }

    // Calculate the total transportation cost
    static long long calculateTotalCost(const std::vector<std::vector<long long>>& cost,
                                   const std::vector<std::vector<long long>>& alloc) {
        long long totalCost = 0;
        for (long long i = 0; i < cost.size(); i++) {
            for (long long j = 0; j < cost[0].size(); j++) {
                if (alloc[i][j] > 0) {
                    totalCost += alloc[i][j] * cost[i][j];
                }
            }
        }
        return totalCost;
    }

    static void addDummyRowOrColumn(std::vector<std::vector<long long>>& cost,
                                    std::vector<long long>& supply,
                                    std::vector<long long>& demand) {
        long long totalSupply = std::accumulate(supply.begin(), supply.end(), 0LL);
        long long totalDemand = std::accumulate(demand.begin(), demand.end(), 0LL);

        // if the total supply is bigger than the total demand we add a column
        if (totalSupply > totalDemand) {
            cost.push_back(std::vector<long long>(demand.size(), 0));
            supply.push_back(totalSupply - totalDemand);
        }
        else if (totalDemand > totalSupply) {
            for (long long i = 0; i < cost[0].size(); i++) {
                cost[i].push_back(0);
            }
            demand.push_back(totalDemand - totalSupply);
        }
    }

    // North-West Corner Rule implementation
    static long long northWestCornerRule(const std::vector<std::vector<long long>>& cost,
                                    std::vector<std::vector<long long>>& alloc,
                                    std::vector<long long>& supply,
                                    std::vector<long long>& demand) {
        long long i = 0, j = 0, initCost = 0;

        while (i < supply.size() && j < demand.size()) {
            long long allocation = std::min(supply[i], demand[j]);
            alloc[i][j] = allocation;
            supply[i] -= allocation;
            demand[j] -= allocation;
            initCost += cost[i][j] * allocation;
            if (supply[i] == 0) i++;
            if (demand[j] == 0) j++;
        }

        // Print initial allocation matrix
        printMatrix(alloc, "Initial Allocation (North-West Corner Rule)");
        std::cout << "Initial Cost: " << initCost << std::endl;
        
        return initCost;
    }

    // MODI method for optimization
    static void modiMethod(const std::vector<std::vector<long long>>& cost,
                           std::vector<std::vector<long long>>& alloc) {
        long long n = cost.size();
        long long m = cost[0].size();
        std::vector<long long> u(n);
        std::vector<long long> v(m);
        std::vector<std::vector<long long>> delta(n, std::vector<long long>(m, 0));

        while (true) {
            // Calculate u and v potentials
            calculateUV(cost, alloc, u, v);

            // Find delta values
            findDelta(cost, alloc, u, v, delta);

            // Find most negative delta
            std::vector<long long> mostNegative = findMostNegativeDelta(delta);

            if (mostNegative[0] == -1) {
                std::cout << "Optimal solution reached!\n";
                std::cout << "Total Optimal Cost: " << calculateTotalCost(cost, alloc) << std::endl;
                return;
            }

            // Find and adjust allocations along loop (simplified)
            std::vector<std::vector<long long>> loop = findLoop(alloc, mostNegative[0], mostNegative[1]);
            adjustAllocations(alloc, loop);
        }
    }

    // Main function to solve transportation problem
    static void solveTransportationProblem() {
        long long numSources, numDestinations;

        std::cout << "Enter the number of sources: ";
        std::cin >> numSources;
        std::cout << "Enter the number of destinations: ";
        std::cin >> numDestinations;

        // Cost matrix input
        std::vector<std::vector<long long>> cost(numSources, std::vector<long long>(numDestinations));
        std::cout << "Enter the cost matrix (row by row):\n";
        for (long long i = 0; i < numSources; i++) {
            for (long long j = 0; j < numDestinations; j++) {
                std::cin >> cost[i][j];
            }
        }
        printMatrix(cost, "Cost");

        // Supply array input
        std::vector<long long> supply(numSources);
        std::cout << "Enter the supply array:\n";
        for (long long i = 0; i < numSources; i++) {
            std::cin >> supply[i];
        }
        printVector(supply, "Supply");

        // Demand array input
        std::vector<long long> demand(numDestinations);
        std::cout << "Enter the demand array:\n";
        for (long long i = 0; i < numDestinations; i++) {
            std::cin >> demand[i];
        }
        printVector(demand, "Demand");

        // Initialize allocation matrix
        std::vector<std::vector<long long>> alloc(numSources, std::vector<long long>(numDestinations, 0));

        // Apply North-West Corner Rule
        northWestCornerRule(cost, alloc, supply, demand);

        // Optimize with MODI method
        modiMethod(cost, alloc);
    }
};

int main() {
    TransportationProblem::solveTransportationProblem();
    return 0;
}
