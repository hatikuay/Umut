#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>     // for sort
#include <numeric>       // for accumulate
#include <unordered_map> // for mode calculation
#include <iomanip>

using namespace std;

const int MAX_SIZE = 100; // Maksimum array boyutu

bool readFile(const std::string &filename, double values[], int &size);
void mergeSort(double array[], int left, int right);
void merge(double array[], int left, int mid, int right);
double calculateMean(const double values[], int size);
double calculateMedian(const double values[], int size);
double calculateMode(const double values[], int size);
void writeOutput(const double values[], int size, double mean, double median, double mode);

int main()
{
    double allValues[MAX_SIZE];
    int totalValues = 0;
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
            if (totalValues == 0)
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

        double tempValues[MAX_SIZE];
        int tempSize = 0;

        if (!readFile(filename, tempValues, tempSize))
        {
            i--; // Hatalı dosya, tekrar denenecek
        }
        else
        {
            std::cout << "The list of " << tempSize << " values in file " << filename << " is :" << std::endl;
            for (int j = 0; j < tempSize; j++)
            {
                std::cout << tempValues[j] << std::endl;
            }
            std::cout << std::endl;

            // Tüm değerleri allValues dizisine ekle
            if (totalValues + tempSize > MAX_SIZE)
            {
                std::cout << "Error: Maximum array size exceeded." << std::endl;
                return -1;
            }
            for (int j = 0; j < tempSize; j++)
            {
                allValues[totalValues++] = tempValues[j];
            }
        }
    }

    std::cout << "*** Summarized Statistics ***" << std::endl;
    std::cout << "The orderly sorted list of " << totalValues << " values is:" << std::endl;
    std::cout << std::endl;

    if (totalValues > 0)
    {
        mergeSort(allValues, 0, totalValues - 1);

        for (int i = 0; i < totalValues; i++)
        {
            std::cout << allValues[i] << " ";
        }
        std::cout << std::endl;

        double mean = calculateMean(allValues, totalValues);
        double median = calculateMedian(allValues, totalValues);
        double mode = calculateMode(allValues, totalValues);

        std::cout << std::endl;
        std::cout << "The mean is " << mean << std::endl;
        std::cout << "The median is " << median << std::endl;
        std::cout << "The mode is " << mode << std::endl;

        writeOutput(allValues, totalValues, mean, median, mode);
    }
    return 0;
}

bool readFile(const std::string &filename, double values[], int &size)
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
        double value;
        if (ss >> value)
        {
            if (size >= MAX_SIZE)
            {
                std::cout << "Error: Exceeded maximum array size." << std::endl;
                file.close();
                return false;
            }
            values[size++] = value;
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

void mergeSort(double array[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void merge(double array[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    double leftArray[MAX_SIZE], rightArray[MAX_SIZE];

    for (int i = 0; i < n1; ++i)
        leftArray[i] = array[left + i];
    for (int j = 0; j < n2; ++j)
        rightArray[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (leftArray[i] <= rightArray[j])
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

double calculateMean(const double values[], int size)
{
    double sum = std::accumulate(values, values + size, 0.0);
    return sum / size;
}

double calculateMedian(const double values[], int size)
{
    if (size % 2 == 0)
    {
        return (values[size / 2 - 1] + values[size / 2]) / 2;
    }
    else
    {
        return values[size / 2];
    }
}

double calculateMode(const double values[], int size)
{
    unordered_map<double, int> frequencyMap;
    for (int i = 0; i < size; i++)
    {
        frequencyMap[values[i]]++;
    }

    int maxCount = 0;
    double mode = values[0];

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

void writeOutput(const double values[], int size, double mean, double median, double mode)
{
    std::ofstream outFile("output_phase1.txt");
    if (outFile.is_open())
    {

        outFile << "*** Summarized Statistics ***" << std::endl;
        outFile << "The orderly sorted list of " << size << " values is:" << std::endl;
        outFile << std::endl;

        for (int i = 0; i < size; i++)
        {
            outFile << values[i] << " ";
        }
        outFile << std::endl;

        outFile << std::endl;
        outFile << "The mean is " << std::fixed << std::setprecision(3) << mean << std::endl;
        outFile << "The median is " << median << std::endl;
        outFile << "The mode is " << mode << std::endl;

        outFile.close();
        std::cout << "Results saved to output_phase1.txt." << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}
