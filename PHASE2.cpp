#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>     // for sort
#include <numeric>       // for accumulate
#include <unordered_map> // for mode calculation
#include <iomanip>
#include <vector>

using namespace std;

struct DataPoint
{
    double value;
    int hour;
    int minute;
    int second;

    bool operator<(const DataPoint &other) const
    {
        if (hour != other.hour)
            return hour < other.hour;
        if (minute != other.minute)
            return minute < other.minute;
        return second < other.second;
    }
};

// const int MAX_SIZE = 100; // Maksimum array boyutu

bool readFile(const std::string &filename, vector<DataPoint> &dataPoints);
void mergeSortByValue(vector<DataPoint> &array, int left, int right);
void merge(vector<DataPoint> &array, int left, int mid, int right);
double calculateMean(const vector<DataPoint> &values);
double calculateMedian(const vector<DataPoint> &values);
double calculateMode(const vector<DataPoint> &values);
void writeOutput(const vector<DataPoint> &values, double mean, double median, double mode);
void sortChoronologically(vector<DataPoint> &values);

int main()
{
    vector<DataPoint> allValues;
    int numFiles;

    std::cout << "*** Welcome to Umut's Data Analyzer ***" << std::endl;
    std::cout << "Enter the number of files to read: ";
    std::cin >> numFiles;
    std::cout << std::endl;

    for (int i = 0; i < numFiles; ++i)
    {
        std::string filename;
        std::cout << "Enter the filename for file " << (i + 1) << ": ";
        std::cin >> filename;

        if (filename == "quit")
        {
            if (allValues.empty())
            {
                std::cout << "Input cancelled. Proceeding to calculation..." << std::endl;
                std::cout << std::endl;
                std::cout << "*** Goodbye. ***" << std::endl;
                return 0;
            }
            else
            {
                std::cout << "Input cancelled. Proceeding to calculation..." << std::endl;
                std::cout << std::endl;
                break;
            }
        }

        vector<DataPoint> tempValues;

        if (!readFile(filename, tempValues))
        {
            i--; // Hatalı dosya, tekrar denenecek
        }
        else
        {
            std::cout << "The list of " << tempValues.size() << " values in file " << filename << " is :" << std::endl;
            for (int j = 0; j < tempValues.size(); j++)
            {
                std::cout << tempValues[j].value << "\t" << setfill('0') << setw(2) << tempValues[j].hour << ":"
                          << setfill('0') << setw(2) << tempValues[j].minute << ":"
                          << setfill('0') << setw(2) << tempValues[j].second << std::endl;
            }
            std::cout << std::endl;
            allValues.insert(allValues.end(), tempValues.begin(), tempValues.end());
        }
    }

    std::cout << "*** Summarized Statistics ***" << std::endl;
    std::cout << "The orderly sorted list of " << allValues.size() << " values is:" << std::endl;
    std::cout << std::endl;

    if (!allValues.empty())
    {
        mergeSortByValue(allValues, 0, allValues.size() - 1);

        for (int i = 0; i < allValues.size(); i++)
        {
            std::cout << allValues[i].value << " ";
        }
        std::cout << std::endl;

        double mean = calculateMean(allValues);
        double median = calculateMedian(allValues);
        double mode = calculateMode(allValues);

        std::cout << std::endl;
        std::cout << "The mean is " << mean << std::endl;
        std::cout << "The median is " << median << std::endl;
        std::cout << "The mode is " << mode << std::endl;
        std::cout << std::endl;

        std::cout << "The chronologically sorted list of " << allValues.size() << " values is:" << std::endl;
        std::cout << std::endl;
        sortChoronologically(allValues);

        for (int i = 0; i < allValues.size(); i++)
        {
            std::cout << allValues[i].value << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        median = calculateMedian(allValues);

        std::cout << std::endl;
        std::cout << "The mean is " << mean << std::endl;
        std::cout << "The median is " << median << std::endl;
        std::cout << "The mode is " << mode << std::endl;

        writeOutput(allValues, mean, median, mode);
    }
    return 0;
}

bool readFile(const std::string &filename, vector<DataPoint> &dataPoints)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "File not found. Please try again." << std::endl;
        return false;
    }

    std::string line;
    bool hasData = false;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        DataPoint data;
        char delim;

        if (ss >> data.value >> data.hour >> delim >> data.minute >> delim >> data.second)
        {
            dataPoints.push_back(data);
            hasData = true;
        }
        else
        {
            std::cout << "Not an input file. Illegal content/structure detected. Please try again." << std::endl;
            file.close();
            return false;
        }

        std::string remaining;
        if (ss >> remaining)
        {
            std::cout << "Not an input file. Illegal content/structure detected. Please try again." << std::endl;
            file.close();
            return false;
        }
    }

    if (!hasData)
    {
        std::cout << "Not an input file. Illegal content/structure detected. Please try again." << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

void mergeSortByValue(vector<DataPoint> &array, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortByValue(array, left, mid);
        mergeSortByValue(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void merge(vector<DataPoint> &array, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<DataPoint> leftArray(n1), rightArray(n2);

    for (int i = 0; i < n1; ++i)
        leftArray[i] = array[left + i];
    for (int j = 0; j < n2; ++j)
        rightArray[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (leftArray[i].value <= rightArray[j].value)
        {
            array[k] = leftArray[i];
            i++;
        }
        else
        {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        array[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        array[k] = rightArray[j];
        j++;
        k++;
    }
}

double calculateMean(const vector<DataPoint> &values)
{
    double sum = 0;
    for (const auto &dp : values)
    {
        sum += dp.value;
    }
    return sum / values.size();
}

double calculateMedian(const vector<DataPoint> &values)
{
    int size = values.size();
    if (size % 2 == 0)
    {
        return (values[size / 2 - 1].value + values[size / 2].value) / 2;
    }
    else
    {
        return values[size / 2].value;
    }
}

double calculateMode(const vector<DataPoint> &values)
{
    unordered_map<double, int> frequencyMap;
    for (const auto &dp : values)
    {
        frequencyMap[dp.value]++;
    }

    int maxCount = 0;
    double mode = values[0].value;

    for (const auto &entry : frequencyMap)
    {
        if (entry.second > maxCount)
        {
            maxCount = entry.second;
            mode = entry.first;
        }
    }

    return mode;
}

void writeOutput(const vector<DataPoint> &values, double mean, double median, double mode)
{
    std::ofstream outFile("output_phase2.txt");
    if (outFile.is_open())
    {

        outFile << "*** Summarized Statistics ***" << std::endl;
        outFile << "The orderly sorted list of " << values.size() << " values is:" << std::endl;
        outFile << std::endl;

        for (const auto &dp : values)
        {
            outFile << dp.value << " ";
        }
        outFile << std::endl;

        outFile << std::endl;
        outFile << "The mean is " << std::fixed << std::setprecision(3) << mean << std::endl;
        outFile << "The median is " << median << std::endl;
        outFile << "The mode is " << mode << std::endl;

        outFile << std::endl;
        outFile << "The chronologically sorted list of " << values.size() << " values is:" << std::endl;
        outFile << std::endl;

        for (const auto &dp : values)
        {
            outFile << dp.value << " ";
        }
        outFile << std::endl;

        mean = calculateMean(values);
        median = calculateMedian(values);
        mode = calculateMode(values);

        outFile << std::endl;
        outFile << "The mean is " << std::fixed << std::setprecision(3) << mean << std::endl;
        outFile << "The median is " << median << std::endl;
        outFile << "The mode is " << mode << std::endl;

        outFile.close();
        std::cout << "Results saved to output_phase2.txt." << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

void sortChoronologically(vector<DataPoint> &values)
{
    sort(values.begin(), values.end());
}
