#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <cmath>

struct Outcome {
    int       matches;  
    long long ways;     
    double    prize;    
};

long long combinations(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    long long result = 1;
    for (int i = 1; i <= k; ++i) {
        result = result * (n - k + i) / i;   
    }
    return result;
}

double prizeForMatches(int k) {
    switch (k) {
        case 12: case 0:  return 250000.0;
        case 11: case 1:  return 500.0;
        case 10: case 2:  return 50.0;
        case 9:  case 3:  return 10.0;
        case 8:  case 4:  return 2.0;
        default:          return 0.0;      
    }
}

std::vector<Outcome> buildOutcomes(int picked, int pool) {
    std::vector<Outcome> outcomes;
    for (int k = 0; k <= picked; ++k) {
        long long ways = combinations(picked, k) * combinations(pool - picked, picked - k);
        outcomes.push_back({k, ways, prizeForMatches(k)});
    }
    return outcomes;
}

void buildDistribution(const std::vector<Outcome>& outcomes,
                        long long totalCombinations,
                        std::vector<double>& X,
                        std::vector<double>& P) {
    std::map<double, long long, std::greater<double>> waysByPrize;
    for (const auto& o : outcomes) waysByPrize[o.prize] += o.ways;

    X.clear();
    P.clear();
    for (const auto& item : waysByPrize) {
        X.push_back(item.first);
        P.push_back(static_cast<double>(item.second) / static_cast<double>(totalCombinations));
    }
}

double sumProbabilities(const std::vector<double>& P) {
    double sum = 0.0;
    for (double p : P) sum += p;
    return sum;
}

// Математичне сподівання
double expectedValue(const std::vector<double>& X, const std::vector<double>& P) {
    double mean = 0.0;
    for (size_t i = 0; i < X.size(); ++i) mean += X[i] * P[i];
    return mean;
}

// Другий початковий момент
double secondMoment(const std::vector<double>& X, const std::vector<double>& P) {
    double m2 = 0.0;
    for (size_t i = 0; i < X.size(); ++i) m2 += X[i] * X[i] * P[i];
    return m2;
}


double variance(double meanX, double meanX2) {
    return meanX2 - meanX * meanX;
}

double stdDeviation(double varianceX) {
    return std::sqrt(varianceX);
}

void printOutcomes(const std::vector<Outcome>& outcomes, long long total) {
    std::cout << "Результат гри за кількістю збігів k\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::setw(4)  << "k"
               << std::setw(14) << "ways"
               << std::setw(12) << "X, $"
               << std::setw(16) << "odds 1 : ..." << "\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto& o : outcomes) {
        std::cout << std::fixed
                   << std::setw(4)  << o.matches
                   << std::setw(14) << o.ways
                   << std::setw(12) << std::setprecision(0) << o.prize
                   << std::setw(16) << std::setprecision(0)
                   << static_cast<double>(total) / static_cast<double>(o.ways) << "\n";
    }
    std::cout << std::string(60, '-') << "\n\n";
}

void printDistributionTable(const std::vector<double>& X, const std::vector<double>& P) {
    std::cout << std::string(40, '-') << "\n";
    std::cout << std::setw(12) << "X, $" << std::setw(18) << "P(X)" << "\n";
    std::cout << std::string(40, '-') << "\n";
    for (size_t i = 0; i < X.size(); ++i) {
        std::cout << std::fixed
                   << std::setw(12) << std::setprecision(0) << X[i]
                   << std::setw(18) << std::setprecision(10) << P[i] << "\n";
    }
    std::cout << std::string(40, '-') << "\n";
}

void printVerification(double sumP) {
    std::cout << "Нормування = "
               << std::setprecision(10) << sumP << "\n\n";
}

void printCharacteristics(double meanX, double meanX2, double varianceX, double stdDevX) {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "M(X)    = " << meanX     << " $\n";
    std::cout << "M(X^2)  = " << meanX2    << "\n";
    std::cout << "D(X)    = " << varianceX << "\n";
    std::cout << "s(X)    = " << stdDevX   << " $\n\n";
}

void printConclusion(double meanX, double ticketPrice) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Ціна квитка: " << ticketPrice << " $\n";
    std::cout << "Виграш в середньому становить: " << (meanX / ticketPrice * 100.0)
               << "% Від ціни квитка\n";
    if (meanX < ticketPrice) {
        std::cout << "Висновок: В середньому виграш в " << (ticketPrice / meanX)
                   << " разів менше,за ціну. приблизна втрата грошей за одну гру = "
                   << (ticketPrice - meanX) << " $.\n";
    } 
}

int main() 
{
    const int picked = 12;    
    const int pool = 24;    
    const double ticketPrice = 2.0;   

    long long total = combinations(pool, picked);   // C(24,12) = 2 704 156
    std::cout << "C(" << pool << "," << picked << ") = " << total << "\n\n";

    std::vector<Outcome> outcomes = buildOutcomes(picked, pool);
    printOutcomes(outcomes, total);

    std::vector<double> X, P;
    buildDistribution(outcomes, total, X, P);
    printDistributionTable(X, P);

    printVerification(sumProbabilities(P));

    double meanX     = expectedValue(X, P);
    double meanX2    = secondMoment(X, P);
    double varianceX = variance(meanX, meanX2);
    double stdDevX   = stdDeviation(varianceX);
    printCharacteristics(meanX, meanX2, varianceX, stdDevX);

    printConclusion(meanX, ticketPrice);

    return 0;
}
