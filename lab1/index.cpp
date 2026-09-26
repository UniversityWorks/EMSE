#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

struct PrizeTier {
    double    amount;  
    long long count;   
};

std::vector<PrizeTier> getPrizeStructure() {
    return {
        {250000.0,      5},
        { 10000.0,     12},
        {  1000.0,    270},
        {   500.0,   7711},
        {   100.0, 133292},
        {    50.0, 163986},
        {    30.0, 327972},
        {    20.0, 574008},
        {    10.0, 901828}
    };
}

void addNoPrizeTier(std::vector<PrizeTier>& tiers, long long totalTickets) {
    long long winningTickets = 0;
    for (const auto& t : tiers) winningTickets += t.count;

    long long noPrizeTickets = totalTickets - winningTickets;
    tiers.push_back({0.0, noPrizeTickets});
}

// Побудова закону розподілу
void buildDistribution(const std::vector<PrizeTier>& tiers,
                        long long totalTickets,
                        std::vector<double>& X,
                        std::vector<double>& P) {
    X.clear();
    P.clear();
    for (const auto& t : tiers) {
        X.push_back(t.amount);
        P.push_back(static_cast<double>(t.count) / static_cast<double>(totalTickets));
    }
}

// Перевірка коректності закону розподілу
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

// Дисперсія
double variance(double meanX, double meanX2) {
    return meanX2 - meanX * meanX;
}

// Середньоквадратичне відхилення
double stdDeviation(double varianceX) {
    return std::sqrt(varianceX);
}

void printDistributionTable(const std::vector<PrizeTier>& tiers,
                             const std::vector<double>& X,
                             const std::vector<double>& P) {
    std::cout << std::fixed;
    std::cout << "Закон розподілу випадкової величини X (сума виграшу, $)\n";
    std::cout << std::string(62, '-') << "\n";
    std::cout << std::setw(12) << "X, " << std::setw(20) << "Кiлькiсть $"
               << std::setw(16) << "P(X)" << "\n";
    std::cout << std::string(62, '-') << "\n";
    for (size_t i = 0; i < X.size(); ++i) {
        std::cout << std::setw(12) << std::setprecision(0) << X[i]
                   << std::setw(16) << tiers[i].count
                   << std::setw(16) << std::setprecision(8) << P[i] << "\n";
    }
    std::cout << std::string(62, '-') << "\n";
}

void printVerification(double sumP) {
    std::cout << "Перевiрка: сума ймовiрностей = "
               << std::setprecision(10) << sumP << "\n\n";
}

void printCharacteristics(double meanX, double meanX2, double varianceX, double stdDevX) {
    std::cout << std::setprecision(4);
    std::cout << "Математичне сподiвання  M(X)   = " << meanX     << " $\n";
    std::cout << "M(X^2)                          = " << meanX2    << "\n";
    std::cout << "Дисперсiя               D(X)   = " << varianceX << "\n";
    std::cout << "Середньокв. вiдхилення  s(X)   = " << stdDevX   << " $\n\n";
}

void printConclusion(double meanX, double ticketPrice) {
    std::cout << "Цiна квитка: " << std::setprecision(2) << ticketPrice << " $\n";
    std::cout << "Середнiй виграш становить " << std::setprecision(2)
               << (meanX / ticketPrice * 100.0) << "% вiд цiни квитка.\n";

    if (meanX < ticketPrice) {
        std::cout << "Висновок: середнiй виграш менший за цiну квитка приблизно у "
                   << std::setprecision(2) << (ticketPrice / meanX) << " раза(ів).\n";
    } else {
        std::cout << "Висновок: середнiй виграш не менший за цiну квитка.\n";
    }
}

int main() {
    const double    ticketPrice   = 10.0;         
    const long long totalTickets  = 8199300;      

    std::vector<PrizeTier> tiers = getPrizeStructure();
    addNoPrizeTier(tiers, totalTickets);

    std::vector<double> X, P;
    buildDistribution(tiers, totalTickets, X, P);
    printDistributionTable(tiers, X, P);

    double sumP = sumProbabilities(P);
    printVerification(sumP);

    double meanX     = expectedValue(X, P);
    double meanX2     = secondMoment(X, P);
    double varianceX = variance(meanX, meanX2);
    double stdDevX   = stdDeviation(varianceX);
    printCharacteristics(meanX, meanX2, varianceX, stdDevX);

    printConclusion(meanX, ticketPrice);

    return 0;
}
